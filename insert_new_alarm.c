#include "ds1307.h"
#include "i2c.h"
#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "external_eeprom.h"
#include "alarm.h"

Alarm *head_once = NULL;
Alarm *head_daily = NULL;
Alarm *head_weekly = NULL;

void insert_new_alarm(void)
{
    while(read_switches(LEVEL_CHANGE) != 0xFF);
    unsigned char alarm_type = set_alarm_type();
    if(alarm_type == 'O')
    {
        while(read_switches(LEVEL_CHANGE) != 0xFF);
        Alarm *temp = set_alarm(alarm_type);
        if(temp != NULL)
        {
            insert_once_alarm(temp);
        }
    }
    else if(alarm_type == 'D')
    {
        while(read_switches(LEVEL_CHANGE) != 0xFF);
        Alarm *temp = set_alarm(alarm_type);
        if(temp != NULL)
        {
            insert_daily_alarm(temp);
        }
    }
//    else if(alarm_type == 'W')
//    {
//        Alarm *temp = set_weekly_alarm(alarm_type);
//    }   
}

void insert_daily_alarm(Alarm *new)
{
    if(head_daily == NULL)
    {
        head_daily = new;
        return;
    }

    Alarm *temp = head_daily;

    if((new->hour < temp->hour) ||
       (new->hour == temp->hour && new->minute < temp->minute))
    {
        new->next = head_daily;
        head_daily->prev = new;
        head_daily = new;
        return;
    }

    while(temp->next != NULL)
    {
        if((new->hour < temp->next->hour) ||
           (new->hour == temp->next->hour &&
            new->minute < temp->next->minute))
        {
            new->next = temp->next;
            new->prev = temp;
            temp->next->prev = new;
            temp->next = new;
            return;
        }

        temp = temp->next;
    }

    temp->next = new;
    new->prev = temp;
}

void insert_once_alarm(Alarm *new)
{
    Alarm *temp = head_once;

    if(head_once == NULL)
    {
        head_once = new;
        return;
    }

    while(temp != NULL)
    {
        if(
            (new->year < temp->year) || 
            (new->year == temp->year && new->month < temp->month) ||
            (new->year == temp->year && new->month == temp->month && new->day < temp->day) ||
            (new->year == temp->year &&  new->month == temp->month && new->day == temp->day && new->hour < temp->hour) ||
            (new->year == temp->year && new->month == temp->month && new->day == temp->day && new->hour == temp->hour &&new->minute < temp->minute))
        {
            new->next = temp;
            new->prev = temp->prev;

            if(temp->prev != NULL)
                temp->prev->next = new;
            else
                head_once = new;

            temp->prev = new;
            return;
        }

        temp = temp->next;
    }

    temp = head_once;

    while(temp->next != NULL)
        temp = temp->next;

    temp->next = new;
    new->prev = temp;
    new->next = NULL;
}