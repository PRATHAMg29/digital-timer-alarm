#ifndef MAIN_H
#define MAIN_H

#include "clcd.h"
#include "ds1307.h"
#include "i2c.h"
#include "main.h"
#include "matrix_keypad.h"
#include "external_eeprom.h"

typedef enum
{
    HOME_SCREEN,
    MENU_SCREEN,
    SET_VIEW_EVENT_SCREEN,
    SET_TIME_DATE_SCREEN,
    ALARM_RING_SCREEN
}Screen;

typedef struct
{
    unsigned int delay;
} KeyStatus;

extern Screen screen;
extern unsigned char seconds;
extern unsigned char event;

unsigned char clock_reg[3];
unsigned char calender_reg[4];
unsigned char time[6];
unsigned char date[11];

void init_ds1307(void);
void display_time(void);
void get_time(void);
void get_date(void);
void display_date(void);
void init_timer0(void);
void menu(unsigned char key);
void wait_for_key_release(void);
void home_screen(unsigned char key);
void set_view_event(unsigned char key);
void set_time_date(unsigned char key);
unsigned char key_event(unsigned char key, unsigned char sw, KeyStatus *status, unsigned int hold_time);

#endif