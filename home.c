#include <xc.h>
#include "clcd.h"
#include "ds1307.h"
#include "i2c.h"
#include "main.h"
#include "matrix_keypad.h"
#include "timer.h"
#include "external_eeprom.h"
#include "alarm.h"

void home_screen(unsigned char key)
{
    static KeyStatus sw1 = {0};

    switch(key_event(key, MK_SW1, &sw1, 500))
    {
        case 2:
            CLEAR_DISP_SCREEN;
            screen = MENU_SCREEN;
            return;

        default:
            break;
    }

    if(event == 0)
    {
        clcd_print("TIME:", LINE1(0));
        clcd_print("DATE:", LINE2(0));

        get_time();
        display_time();

        get_date();
        display_date();
    }
    else
    {
        display_alarm();

        if(seconds >= 2)
        {
            seconds = 0;
            event = 0;
            CLEAR_DISP_SCREEN;
        }
    }
}

