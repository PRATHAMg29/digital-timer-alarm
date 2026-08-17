#include "ds1307.h"
#include "i2c.h"
#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "external_eeprom.h"
#include "alarm.h"

Alarm *set_alarm(unsigned char alarm_type)
{
    CLEAR_DISP_SCREEN;
    static KeyStatus sw1 = {0}, sw2 = {0};
    unsigned char hour, minute;
    hour = ((time[0] - '0') * 10) + (time[1] - '0');
    minute = ((time[3] - '0') * 10) + (time[4] - '0');
    clcd_print(" SET ONCE ALARM ", LINE1(0));

    unsigned char update_time[6];
    update_time[0] = (hour / 10) + '0';
    update_time[1] = (hour % 10) + '0';
    update_time[2] = ':';
    update_time[3] = (minute / 10) + '0';
    update_time[4] = (minute % 10) + '0';
    update_time[5] = '\0';

    unsigned char edit = 0;

    Alarm *new = (Alarm *)malloc(sizeof(Alarm));
    if(new == NULL)
    {
        return NULL;
    }

    if(alarm_type == 'D')
    {
        clcd_print(" SET DAILY ALARM", LINE1(0));
    }
    else if(alarm_type == 'O')
    {
        clcd_print(" SET ONCE ALARM ", LINE1(0));
    }

    while(1)
    {
        unsigned char key = read_switches(LEVEL_CHANGE);

        switch(key_event(key, MK_SW1, &sw1, 500))
        {
            case 1:
                if(edit == 0)
                {
                    if(hour < 23)
                        hour++;
                }
                else if(edit == 1)
                {
                    if(minute < 59)
                        minute++;
                }
                break;

            case 2:
                if(edit < 2)
                {
                    while(read_switches(LEVEL_CHANGE) != 0xFF);
                    edit++;
                }
                else
                {
                    new->hour = hour;
                    new->minute = minute;

                    if(alarm_type == 'D')
                    {
                        new->prev = NULL;
                        new->next = NULL;
                        new->day = 0;
                        new->month = 0;
                        new->year = 0;
                        new->id = next_alarm_id++;
                        new->type = alarm_type;
                        return new;
                    }
                    else
                    {
                        goto SET_DATE;
                    }
                }
                break;

            default:
                break;
        }

        switch(key_event(key, MK_SW2, &sw2, 500))
        {
            case 1:
                if(edit == 0)
                {
                    if(hour > 0)
                        hour--;
                }
                else if(edit == 1)
                {
                    if(minute > 0)
                        minute--;
                }
                break;

            case 2:
                free(new);
                return NULL;

            default:
                break;
        }

        update_time[0] = (hour / 10) + '0';
        update_time[1] = (hour % 10) + '0';
        update_time[2] = ':';
        update_time[3] = (minute / 10) + '0';
        update_time[4] = (minute % 10) + '0';
        update_time[5] = '\0';
        if(seconds % 2)
        {
            if(edit == 0)          // Editing hour
            {
                update_time[0] = ' ';
                update_time[1] = ' ';
            }
            else if(edit == 1)     // Editing minute
            {
                update_time[3] = ' ';
                update_time[4] = ' ';
            }
        }
        clcd_print(update_time, LINE2(2));
    }

SET_DATE:
     
    CLEAR_DISP_SCREEN;
    clcd_print("   SET DATE     ", LINE1(0));
    while(read_switches(LEVEL_CHANGE) != 0xFF);
    edit = 0;

    unsigned char day = 0, month = 0;
    unsigned int year = 0;

    day = ((date[0] - '0') * 10) + (date[1] - '0');
    month = ((date[3] - '0') * 10) + (date[4] - '0');
    year = ((date[6] - '0') * 1000) + ((date[7] - '0') * 100) + ((date[8] - '0') * 10) + (date[9] - '0');

    unsigned char update_date[11];
    update_date[0] = (day / 10) + '0';
    update_date[1] = (day % 10) + '0';
    update_date[2] = '-';
    update_date[3] = (month / 10) + '0';
    update_date[4] = (month % 10) + '0';
    update_date[5] = '-';
    update_date[6] = (year / 1000) + '0';
    update_date[7] = ((year / 100) % 10) + '0';
    update_date[8] = ((year / 10) % 10) + '0';
    update_date[9] = (year % 10) + '0';
    update_date[10] = '\0';
    
    while(1)
    {
        unsigned char key = read_switches(LEVEL_CHANGE);

        switch(key_event(key, MK_SW1, &sw1, 500))
        {
            case 1:
                if(edit == 0)
                {
                    if(day < 31)
                        day++;
                }
                else if(edit == 1)
                {
                    if(month < 12)
                        month++;
                }
                else if(edit == 2)
                {
                    if(year < 2050)
                        year++;
                }
                break;

            case 2:
                if(edit < 3)
                {
                    while(read_switches(LEVEL_CHANGE) != 0xFF);
                    edit++;
                }
                else
                {
                    new->day = day;
                    new->month = month;
                    new->year = year;
                    new->next = NULL;
                    new->prev = NULL;
                    new->type = alarm_type;
                    new->id = next_alarm_id++;

                    return new;
                }
                break;

            default:
                break;
        }

        switch(key_event(key, MK_SW2, &sw2, 500))
        {
            case 1:
                if(edit == 0)
                {
                    if(day > 0)
                        day--;
                }
                else if(edit == 1)
                {
                    if(month > 0)
                        month--;
                }
                else if(edit == 2)
                {
                    if(year > 2000)
                        year--;
                }
                break;

            case 2:
                free(new);
                return NULL;

            default:
                break;
        }

        update_date[0] = (day / 10) + '0';
        update_date[1] = (day % 10) + '0';
        update_date[2] = '-';
        update_date[3] = (month / 10) + '0';
        update_date[4] = (month % 10) + '0';
        update_date[5] = '-';
        update_date[6] = (year / 1000) + '0';
        update_date[7] = ((year / 100) % 10) + '0';
        update_date[8] = ((year / 10) % 10) + '0';
        update_date[9] = (year % 10) + '0';
        update_date[10] = '\0';
        if(seconds % 2)
        {
            if(edit == 0)          // Day
            {
                update_date[0] = ' ';
                update_date[1] = ' ';
            }
            else if(edit == 1)     // Month
            {
                update_date[3] = ' ';
                update_date[4] = ' ';
            }
            else if(edit == 2)     // Year
            {
                update_date[6] = ' ';
                update_date[7] = ' ';
                update_date[8] = ' ';
                update_date[9] = ' ';
            }
        }
        clcd_print(update_date, LINE2(0));
    }
}