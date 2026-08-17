#include <xc.h>
#include "clcd.h"
#include "ds1307.h"
#include "i2c.h"
#include "main.h"
#include "matrix_keypad.h"
#include "timer.h"
#include "external_eeprom.h"
#include "alarm.h"

void alarm_ring_screen(unsigned char key)
{
    static KeyStatus sw1 = {0};

    PORTBbits.RB0 = 1;

    if(ringing_alarm->type == 'O')
    {
        unsigned char t[6];
        unsigned char d[11];
        clcd_print("*** ALARM ***", LINE1(0));
        t[0] = ringing_alarm->hour / 10 + '0';
        t[1] = ringing_alarm->hour % 10 + '0';
        t[2] = ':';
        t[3] = ringing_alarm->minute / 10 + '0';
        t[4] = ringing_alarm->minute % 10 + '0';
        t[5] = '\0';

        d[0] = ringing_alarm->day / 10 + '0';
        d[1] = ringing_alarm->day % 10 + '0';
        d[2] = '-';
        d[3] = ringing_alarm->month / 10 + '0';
        d[4] = ringing_alarm->month % 10 + '0';
        d[5] = '-';
        d[6] = ringing_alarm->year / 1000 + '0';
        d[7] = (ringing_alarm->year / 100) % 10 + '0';
        d[8] = (ringing_alarm->year / 10) % 10 + '0';
        d[9] = ringing_alarm->year % 10 + '0';
        d[10] = '\0';

        clcd_print("*** ALARM ***", LINE1(0));
        clcd_print(t, LINE2(0));
        clcd_putch(' ', LINE2(5));
        clcd_print(d, LINE2(6));
    }
    else
    {
        unsigned char t[6];
        clcd_print(" DAILY ALARM ", LINE1(0));
        t[0] = ringing_alarm->hour / 10 + '0';
        t[1] = ringing_alarm->hour % 10 + '0';
        t[2] = ':';
        t[3] = ringing_alarm->minute / 10 + '0';
        t[4] = ringing_alarm->minute % 10 + '0';
        t[5] = '\0';
        clcd_print(t, LINE2(0));
    }

    switch(key_event(key, MK_SW1, &sw1, 500))
    {
        case 2:

            PORTEbits.RE0 = 0;
            if(ringing_alarm->type == 'O')
            {
                delete_alarm(ringing_alarm);
            }
            while(read_switches(LEVEL_CHANGE) != 0xFF);
            ringing_alarm = NULL;
            screen = HOME_SCREEN;
            break;
    }
}