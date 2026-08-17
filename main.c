/*
 * File:   main.c
 * Author: prath
 *
 * Created on 12 July, 2026, 8:03 PM
 */


#include <xc.h>
#include "clcd.h"
#include "ds1307.h"
#include "i2c.h"
#include "main.h"
#include "matrix_keypad.h"
#include "timer.h"
#include "external_eeprom.h"
#include "alarm.h"

Screen screen = HOME_SCREEN;
unsigned char seconds = 0;
unsigned char event = 0;
unsigned char clock_reg[3];
unsigned char calender_reg[4];
unsigned char time[6];
unsigned char date[11];
Alarm *ringing_alarm = NULL;

void init_config(void)
{
	PEIE = 1;
	/* Config PORTB as digital */
	ADCON1 = 0x0F;
	init_timer0();
    init_clcd();
	init_i2c();
	init_ds1307();
    init_matrix_keypad();
    TRISEbits.RE0 = 0;
    PORTEbits.RE0 = 0;
	GIE = 1;
}

void main(void) 
{
    init_config();
    while(1)
    {
        unsigned char key = read_switches(LEVEL_CHANGE);
        if(screen == HOME_SCREEN)
        {
            home_screen(key);
        }
        else if(screen == MENU_SCREEN)
        {
            menu(key);
        }
        else if(screen == SET_VIEW_EVENT_SCREEN)
        {
            set_view_event(key);
        }
        else if(screen == SET_TIME_DATE_SCREEN)
        {
            set_time_date(key);
        }
        else if(screen == ALARM_RING_SCREEN)
        {
            alarm_ring_screen(key);
        }
        check_alarm_for_ring();
        
    }
}

void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

	if (clock_reg[0] & 0x40)
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	else
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = '\0';
}

void get_date(void)
{
	calender_reg[0] = read_ds1307(YEAR_ADDR);
	calender_reg[1] = read_ds1307(MONTH_ADDR);
	calender_reg[2] = read_ds1307(DATE_ADDR);
	calender_reg[3] = read_ds1307(DAY_ADDR);

	date[6] = '2';
	date[7] = '0';
	date[8] = '0' + ((calender_reg[0] >> 4) & 0x0F);
	date[9] = '0' + (calender_reg[0] & 0x0F);
	date[5] = '-';
	date[3] = '0' + ((calender_reg[1] >> 4) & 0x0F);
	date[4] = '0' + (calender_reg[1] & 0x0F);
	date[2] = '-';
	date[0] = '0' + ((calender_reg[2] >> 4) & 0x0F);
	date[1] = '0' + (calender_reg[2] & 0x0F);
	date[10] = '\0';
}

void display_date(void)
{
	clcd_print(date, LINE2(5));
}

void display_time(void)
{
	clcd_print(time, LINE1(5));
}

void wait_for_key_release(void)
{
    while(read_switches(LEVEL_CHANGE) != 0);
}

void check_alarm_for_ring(void)
{
    static unsigned char alarm_triggered = 0;
    Alarm *alarm = find_nearest_alarm();
    if(alarm == NULL)
    {
        return;
    }
    get_time();
    get_date();

    unsigned char day = ((date[0]-'0')*10) + (date[1]-'0');

    unsigned char month = ((date[3]-'0')*10) + (date[4]-'0');

    unsigned int year = ((date[6]-'0')*1000) + ((date[7]-'0')*100) + ((date[8]-'0')*10) + (date[9]-'0');
    unsigned char hour;
    unsigned char minute;
    hour = ((time[0] - '0') * 10) + (time[1] - '0');
    minute = ((time[3] - '0') * 10) + (time[4] - '0');
    unsigned char ring = 0;

    if(alarm->type == 'D')
    {
        if(hour == alarm->hour && minute == alarm->minute)
        {
            ring = 1;
        }
    }
    else if(alarm->type == 'O')
    {
        if(year == alarm->year &&
           month == alarm->month &&
           day == alarm->day &&
           hour == alarm->hour &&
           minute == alarm->minute)
        {
            ring = 1;
        }
    }
    
    if(ring)
    {
        if(alarm_triggered == 0)
        {
            PORTEbits.RE0 = 1;
            ringing_alarm = alarm;
            screen = ALARM_RING_SCREEN;
            alarm_triggered = 1;
        }
    }
    else
    {
        alarm_triggered = 0;
        PORTEbits.RE0 = 0;
    }
}