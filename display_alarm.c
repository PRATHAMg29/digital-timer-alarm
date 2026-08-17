#include<xc.h>
#include "clcd.h"
#include "ds1307.h"
#include "i2c.h"
#include "main.h"
#include "matrix_keypad.h"
#include "external_eeprom.h"
#include "alarm.h"

unsigned char display_time_temp[6];
unsigned char display_date_temp[11];
unsigned char next_alarm_id = 1;

void display_alarm(void)
{
    Alarm *alarms = find_nearest_alarm();
    if(alarms == NULL)
    {
        clcd_print("   NO ALARMS    ",LINE1(0));
        clcd_print("                ",LINE2(0));
        return;
    }
    display_time_temp[0] = (alarms->hour/10) + '0';
    display_time_temp[1] = (alarms->hour%10) + '0';
    display_time_temp[2] = ':';
    display_time_temp[3] = (alarms->minute/10) + '0';
    display_time_temp[4] = (alarms->minute%10) + '0';
    display_time_temp[5] = '\0';
     clcd_print("TIME:",LINE1(0));
    clcd_print(display_time_temp,LINE1(5));
    if(alarms->type == 'D')
    {
        clcd_print("   DAILY       ",LINE2(0));
    }
    else
    {
        display_date_temp[0] = (alarms->day/10)+'0';
        display_date_temp[1] = (alarms->day%10)+'0';
        display_date_temp[2] = '-';
        display_date_temp[3] = (alarms->month/10)+'0';
        display_date_temp[4] = (alarms->month%10)+'0';
        display_date_temp[5] = '-';
        display_date_temp[6] = (alarms->year / 1000) + '0';
        display_date_temp[7] = ((alarms->year / 100) % 10) + '0';
        display_date_temp[8] = ((alarms->year / 10) % 10) + '0';
        display_date_temp[9] = (alarms->year % 10) + '0';
        display_date_temp[10] = '\0';
        
        clcd_print("DATE:",LINE2(0));
        clcd_print(display_date_temp,LINE2(5));
    }
    
    clcd_putch(alarms->type,LINE1(15));
    
}