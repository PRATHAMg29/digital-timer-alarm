#ifndef ALARM_H
#define ALARM_H

#include<xc.h>
#include "clcd.h"
#include "ds1307.h"
#include "i2c.h"
#include "main.h"
#include "matrix_keypad.h"
#include "external_eeprom.h"

typedef struct alarm
{
    unsigned char id;
    
    unsigned char day;
    unsigned char month;
    unsigned int year;
    
    unsigned char hour;
    unsigned char minute;

    unsigned char type;
    
    struct alarm *prev;
    struct alarm *next;
}Alarm;

extern Alarm *head_once;
extern Alarm *head_daily;
extern Alarm *head_weekly;
extern Alarm *ringing_alarm;

extern unsigned char once_count;
extern unsigned char daily_count;
extern unsigned char weekly_count;
extern unsigned char next_alarm_id;

void display_alarm(void);
unsigned char set_alarm_type(void);
Alarm *set_alarm(unsigned char alarm_type);
Alarm *find_nearest_alarm(void);
void insert_new_alarm(void);
void insert_once_alarm(Alarm *new);
void insert_daily_alarm(Alarm *new);
void view_event();
//Alarm *set_weekly_alarm(unsigned char alarm_type);
void delete_alarm(Alarm *current);
void check_alarm_for_ring(void);
void alarm_ring_screen(unsigned char key);
void set_time(void);
void set_date(void);

#endif