#include <xc.h>
#include "clcd.h"
#include "ds1307.h"
#include "i2c.h"
#include "main.h"
#include "matrix_keypad.h"
#include "timer.h"
#include "external_eeprom.h"
#include "alarm.h"

void menu(unsigned char key)
{
    static unsigned char menu_index;
    static KeyStatus sw1, sw2;

    switch(key_event(key, MK_SW1, &sw1, 500))
    {
        case 1:
            if(menu_index < 1)
                menu_index++;
            break;

        case 2:
            while(read_switches(LEVEL_CHANGE) != 0xFF);
            CLEAR_DISP_SCREEN;

            if(menu_index == 0)
                screen = SET_VIEW_EVENT_SCREEN;
            else
                screen = SET_TIME_DATE_SCREEN;

            return;
    }

    switch(key_event(key, MK_SW2, &sw2, 500))
    {
        case 1:
            if(menu_index > 0)
                menu_index--;
            break;

        case 2:
            
            CLEAR_DISP_SCREEN;
            screen = HOME_SCREEN;
            return;
    }
    
    if(menu_index == 0)
    {
        clcd_print("=>SET/VIEW EVENT",LINE1(0));
        clcd_print("  SET TIME/DATE ",LINE2(0));
    }
    else
    {
        clcd_print("  SET/VIEW EVENT",LINE1(0));
        clcd_print("=>SET TIME/DATE ",LINE2(0));
    }
}


void set_time_date(unsigned char key)
{
    static unsigned char menu_index = 0;
    static KeyStatus sw1 = {0}, sw2 = {0};
    switch(key_event(key, MK_SW1, &sw1, 500))
    {
        case 1:
            if(menu_index < 1)
                menu_index++;
            break;

        case 2:
            if(menu_index == 0)
            {
                CLEAR_DISP_SCREEN;
                // screen = SET_TIME_SCREEN;
                set_time();
            }
            else
            {
                CLEAR_DISP_SCREEN;
                // screen = SET_DATE_SCREEN;
                set_date();
            }
            return;

        default:
            break;
    }

    switch(key_event(key, MK_SW2, &sw2, 500))
    {
        case 1:
            if(menu_index > 0)
                menu_index--;
            break;

        case 2:
            CLEAR_DISP_SCREEN;
            screen = MENU_SCREEN;
            return;

        default:
            break;
    }
    if(menu_index == 0)
    {
        clcd_print("=>SET TIME      ", LINE1(0));
        clcd_print("  SET DATE      ", LINE2(0));
    }
    else
    {
        clcd_print("  SET TIME      ", LINE1(0));
        clcd_print("=>SET DATE      ", LINE2(0));
    }
}

void set_view_event(unsigned char key)
{
//    while(read_switches(LEVEL_CHANGE) != 0xFF);
    static unsigned char menu_index = 0;
    static KeyStatus sw1 = {0}, sw2 = {0};

    switch(key_event(key, MK_SW1, &sw1, 500))
    {
        case 1:
            if(menu_index < 1)
                menu_index++;
            break;

        case 2:
            while(read_switches(LEVEL_CHANGE) != 0xFF);
            if(menu_index == 0)
            {
                CLEAR_DISP_SCREEN;
                insert_new_alarm();
                CLEAR_DISP_SCREEN;
            }
            else if(menu_index == 1)
            {
                CLEAR_DISP_SCREEN;
                view_event();
                CLEAR_DISP_SCREEN;
            }
            return;

        default:
            break;
    }

    switch(key_event(key, MK_SW2, &sw2, 500))
    {
        case 1:
            if(menu_index > 0)
                menu_index--;
            break;

        case 2:
            while(read_switches(LEVEL_CHANGE) != 0xFF);
            CLEAR_DISP_SCREEN;
            screen = MENU_SCREEN;
            return;

        default:
            break;
    }

    if(menu_index == 0)
    {
        clcd_print("=>SET EVENT     ", LINE1(0));
        clcd_print("  VIEW EVENT    ", LINE2(0));
    }
    else
    {
        clcd_print("  SET EVENT     ", LINE1(0));
        clcd_print("=>VIEW EVENT    ", LINE2(0));
    }
}