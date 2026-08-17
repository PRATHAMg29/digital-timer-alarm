#include "ds1307.h"
#include "i2c.h"
#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "external_eeprom.h"
#include "alarm.h"

unsigned char once_count = 0;
unsigned char daily_count = 0;
unsigned char weekly_count = 0;

unsigned char set_alarm_type(void)
{
    unsigned char index = 0, alarm_type;
    static KeyStatus sw1 = {0}, sw2 = {0};

    while(1)
    {
        unsigned char key = read_switches(LEVEL_CHANGE);

        switch(key_event(key, MK_SW1, &sw1, 400))
        {
            case 1:
                if(index < 2)
                    index++;
                break;

            case 2:
                if(index == 0)
                    alarm_type = 'O';
                else if(index == 1)
                    alarm_type = 'D';
                else
                    alarm_type = 'W';

                return alarm_type;

            default:
                break;
        }

        switch(key_event(key, MK_SW2, &sw2, 400))
        {
            case 1:
                if(index > 0)
                    index--;
                break;

            case 2:
                return 0;

            default:
                break;
        }

        if(index == 0)
        {
            clcd_print("   SET ALARM    ", LINE1(0));
            clcd_print("=>     ONCE     ", LINE2(0));
        }
        else if(index == 1)
        {
            clcd_print("       ONCE     ", LINE1(0));
            clcd_print("=>    DAILY     ", LINE2(0));
        }
        else
        {
            clcd_print("      DAILY     ", LINE1(0));
            clcd_print("=>    WEEKLY    ", LINE2(0));
        }
    }
}
