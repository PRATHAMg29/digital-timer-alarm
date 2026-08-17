#include <xc.h>
#include "clcd.h"
#include "ds1307.h"
#include "i2c.h"
#include "main.h"
#include "matrix_keypad.h"
#include "timer.h"
#include "external_eeprom.h"
#include "alarm.h"

void view_event(void)
{
    unsigned char index = 0;
    static KeyStatus sw1 = {0}, sw2 = {0};

    while(1)
    {
        unsigned char key = read_switches(LEVEL_CHANGE);

        switch(key_event(key, MK_SW1, &sw1, 500))
        {
            case 1:
                if(index < 2)
                    index++;
                break;

            case 2:
                goto SELECTED;
        }

        switch(key_event(key, MK_SW2, &sw2, 500))
        {
            case 1:
                if(index > 0)
                    index--;
                break;

            case 2:
                return;
        }

        if(index == 0)
        {
            clcd_print("=>VIEW ONCE    ", LINE1(0));
            clcd_print("  VIEW DAILY   ", LINE2(0));
        }
        else if(index == 1)
        {
            clcd_print("  VIEW ONCE    ", LINE1(0));
            clcd_print("=>VIEW DAILY   ", LINE2(0));
        }
        else
        {
            clcd_print("  VIEW DAILY   ", LINE1(0));
            clcd_print("=>VIEW WEEKLY  ", LINE2(0));
        }
    }

SELECTED:

    Alarm *current = NULL;

    if(index == 0)
    {
        current = head_once;
    }
    else if(index == 1)
    {
        current = head_daily;
    }
//    else if(index == 2)
//    {
//        current = head_weekly;
//    }

    if(current == NULL)
    {
        CLEAR_DISP_SCREEN;
        clcd_print("NO ALARMS      ", LINE1(0));
        clcd_print("PRESS SW2 EXIT ", LINE2(0));

        while(1)
        {
            unsigned char key = read_switches(LEVEL_CHANGE);

            if(key_event(key, MK_SW2, &sw2, 500) == 2)
            {
                screen = SET_VIEW_EVENT_SCREEN;
                return;
            }
        }
    }

    CLEAR_DISP_SCREEN;

    while(1)
    {
        unsigned char key = read_switches(LEVEL_CHANGE);

        switch(key_event(key, MK_SW1, &sw1, 500))
        {
            case 1:
                if(current->next != NULL)
                    current = current->next;
                break;

            case 2:
            {
                Alarm *next_alarm;

                if(current->next != NULL)
                    next_alarm = current->next;
                else
                    next_alarm = current->prev;

                delete_alarm(current);

                current = next_alarm;

                CLEAR_DISP_SCREEN;

                if(current == NULL)
                {
                    clcd_print("NO ALARMS      ", LINE1(0));
                    clcd_print("PRESS SW2 EXIT ", LINE2(0));

                    while(1)
                    {
                        key = read_switches(LEVEL_CHANGE);

                        if(key_event(key, MK_SW2, &sw2, 500) == 2)
                        {
                            screen = SET_VIEW_EVENT_SCREEN;
                            return;
                        }
                    }
                }

                break;
            }

            default:
                break;
        }

        switch(key_event(key, MK_SW2, &sw2, 500))
        {
            case 1:
                if(current->prev != NULL)
                    current = current->prev;
                break;

            case 2:
                CLEAR_DISP_SCREEN;
                screen = SET_VIEW_EVENT_SCREEN;
                return;

            default:
                break;
        }

        if(current == NULL)
            continue;

        if(current->type == 'O')
        {
            unsigned char id;
            unsigned char time_show[6];
            unsigned char date_show[11];

            id = current->id + '0';

            clcd_print("ID:", LINE1(0));
            clcd_putch(id, LINE1(3));

            time_show[0] = (current->hour / 10) + '0';
            time_show[1] = (current->hour % 10) + '0';
            time_show[2] = ':';
            time_show[3] = (current->minute / 10) + '0';
            time_show[4] = (current->minute % 10) + '0';
            time_show[5] = '\0';

            clcd_print("TIME:", LINE1(6));
            clcd_print(time_show, LINE1(11));

            date_show[0] = (current->day / 10) + '0';
            date_show[1] = (current->day % 10) + '0';
            date_show[2] = '-';
            date_show[3] = (current->month / 10) + '0';
            date_show[4] = (current->month % 10) + '0';
            date_show[5] = '-';
            date_show[6] = (current->year / 1000) + '0';
            date_show[7] = ((current->year / 100) % 10) + '0';
            date_show[8] = ((current->year / 10) % 10) + '0';
            date_show[9] = (current->year % 10) + '0';
            date_show[10] = '\0';

            clcd_print("DATE:", LINE2(0));
            clcd_print(date_show, LINE2(5));
        }
        else if(current->type == 'D')
        {
            unsigned char id;
            unsigned char time_show[6];

            clcd_print("  DAILY ALARM  ", LINE1(0));

            id = current->id + '0';

            clcd_print("ID:", LINE2(0));
            clcd_putch(id, LINE2(3));

            time_show[0] = (current->hour / 10) + '0';
            time_show[1] = (current->hour % 10) + '0';
            time_show[2] = ':';
            time_show[3] = (current->minute / 10) + '0';
            time_show[4] = (current->minute % 10) + '0';
            time_show[5] = '\0';

            clcd_print("TIME:", LINE2(6));
            clcd_print(time_show, LINE2(11));
        }

//        else if(current->type == 'W')
//        {
//            /* Weekly alarm display */
//        }
    }
}