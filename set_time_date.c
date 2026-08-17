#include <xc.h>
#include "clcd.h"
#include "ds1307.h"
#include "i2c.h"
#include "main.h"
#include "matrix_keypad.h"
#include "timer.h"
#include "external_eeprom.h"
#include "alarm.h"

void set_time(void)
{
    CLEAR_DISP_SCREEN;

    static KeyStatus sw1 = {0}, sw2 = {0};

    get_time();

    unsigned char hour = ((time[0]-'0')*10) + (time[1]-'0');
    unsigned char minute = ((time[3]-'0')*10) + (time[4]-'0');

    unsigned char edit = 0;
    unsigned char blink = 0;
    unsigned int blink_delay = 0;

    unsigned char disp[6];

    clcd_print("   SET TIME    ",LINE1(0));

    while(1)
    {
        unsigned char key = read_switches(LEVEL_CHANGE);

        switch(key_event(key,MK_SW1,&sw1,500))
        {
            case 1:
                if(edit==0)
                {
                    if(hour<23) 
                        hour++;
                    else 
                        hour=0;
                }
                else
                {
                    if(minute<59) 
                        minute++;
                    else 
                        minute=0;
                }
                break;

            case 2:
                while(read_switches(LEVEL_CHANGE) != 0xFF);
                if(edit==0)
                    edit++;
                else
                {
                    write_ds1307(HOUR_ADDR,((hour/10)<<4)|(hour%10));
                    write_ds1307(MIN_ADDR,((minute/10)<<4)|(minute%10));
                    screen = HOME_SCREEN;
                    CLEAR_DISP_SCREEN;
                    return;
                }
        }

        switch(key_event(key,MK_SW2,&sw2,500))
        {
            case 1:
                if(edit==0)
                {
                    if(hour>0) 
                        hour--;
                    else 
                        hour=23;
                }
                else
                {
                    if(minute>0) 
                        minute--;
                    else 
                        minute=59;
                }
                break;

            case 2:
                while(read_switches(LEVEL_CHANGE) != 0xFF);
                screen = HOME_SCREEN;
                CLEAR_DISP_SCREEN;
                return;
        }

        disp[0]=(hour/10)+'0';
        disp[1]=(hour%10)+'0';
        disp[2]=':';
        disp[3]=(minute/10)+'0';
        disp[4]=(minute%10)+'0';
        disp[5]='\0';

        blink_delay++;

        if(blink_delay>250)
        {
            blink_delay=0;
            blink=!blink;
        }

        if(edit==0 && blink)
        {
            disp[0]=' ';
            disp[1]=' ';
        }

        if(edit==1 && blink)
        {
            disp[3]=' ';
            disp[4]=' ';
        }

        clcd_print(disp,LINE2(5));
    }
}

void set_date(void)
{
    CLEAR_DISP_SCREEN;

    static KeyStatus sw1={0},sw2={0};

    get_date();

    unsigned char day=((date[0]-'0')*10)+(date[1]-'0');
    unsigned char month=((date[3]-'0')*10)+(date[4]-'0');

    unsigned int year=((date[6]-'0')*1000) + ((date[7]-'0')*100) + ((date[8]-'0')*10) + (date[9]-'0');
    unsigned char edit=0;
    unsigned char disp[11];
    unsigned char blink=0;
    unsigned int blink_delay=0;
    clcd_print("   SET DATE    ",LINE1(0));
    while(1)
    {
        unsigned char key=read_switches(LEVEL_CHANGE);

        switch(key_event(key,MK_SW1,&sw1,500))
        {
            case 1:

                if(edit==0)
                {
                    if(day<31) 
                        day++;
                    else 
                        day=1;
                }
                else if(edit==1)
                {
                    if(month<12) 
                        month++;
                    else 
                        month=1;
                }
                else
                {
                    if(year<2099) 
                        year++;
                    else 
                        year=2000;
                }

                break;

            case 2:

                while(read_switches(LEVEL_CHANGE) != 0xFF);

                if(edit<2)
                    edit++;
                else
                {
                    write_ds1307(DATE_ADDR,((day/10)<<4)|(day%10));
                    write_ds1307(MONTH_ADDR,((month/10)<<4)|(month%10));
                    write_ds1307(YEAR_ADDR,(((year%100)/10)<<4)|(year%10));
                    screen=HOME_SCREEN;
                    CLEAR_DISP_SCREEN;
                    return;
                }
                break;
        }

        switch(key_event(key,MK_SW2,&sw2,500))
        {
            case 1:

                if(edit==0)
                {
                    if(day>1) 
                        day--;
                    else 
                        day=31;
                }
                else if(edit==1)
                {
                    if(month>1) 
                        month--;
                    else 
                        month=12;
                }
                else
                {
                    if(year>2000) 
                        year--;
                    else 
                        year=2099;
                }

                break;

            case 2:

                while(read_switches(LEVEL_CHANGE) != 0xFF);
                screen=HOME_SCREEN;
                CLEAR_DISP_SCREEN;
                return;
        }

        disp[0]=(day/10)+'0';
        disp[1]=(day%10)+'0';
        disp[2]='-';
        disp[3]=(month/10)+'0';
        disp[4]=(month%10)+'0';
        disp[5]='-';
        disp[6]=(year/1000)+'0';
        disp[7]=((year/100)%10)+'0';
        disp[8]=((year/10)%10)+'0';
        disp[9]=(year%10)+'0';
        disp[10]='\0';

        blink_delay++;

        if(blink_delay>250)
        {
            blink_delay=0;
            blink=!blink;
        }

        if(edit==0 && blink)
        {
            disp[0]=' ';
            disp[1]=' ';
        }

        if(edit==1 && blink)
        {
            disp[3]=' ';
            disp[4]=' ';
        }

        if(edit==2 && blink)
        {
            disp[6]=' ';
            disp[7]=' ';
            disp[8]=' ';
            disp[9]=' ';
        }

        clcd_print(disp,LINE2(0));
    }
}