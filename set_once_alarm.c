#include "ds1307.h"
#include "i2c.h"
#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "external_eeprom.h"
#include "alarm.h"

Alarm *set_once_alarm(unsigned char alarm_type)
{
    unsigned int delay_up = 0;
    unsigned int delay_down = 0;
    CLEAR_DISP_SCREEN;
    unsigned char hour,minute;
    hour = ((time[0] - '0') * 10) + (time[1] - '0');
    minute = ((time[3] - '0') * 10) + (time[4] - '0');
//    second = ((time[6] - '0') * 10) + (time[7] - '0');
    clcd_print(" SET ONCE ALARM ",LINE1(0));
    unsigned char update_time[6];
    update_time[0] = (hour / 10) + '0';
    update_time[1] = (hour % 10) + '0';
    update_time[2] = ':';
    update_time[3] = (minute / 10) + '0';
    update_time[4] = (minute % 10) + '0';
    update_time[5] = '\0';
//    update_time[6] = (second / 10) + '0';
//    update_time[7] = (second % 10) + '0';
//    update_time[8] = '\0';
    unsigned char edit = 0;
    Alarm *new = (Alarm *)malloc(sizeof(Alarm));
    if(new == NULL)
    {
        return NULL;
    }
    while(1)
    {    
            unsigned char key = read_switches(LEVEL_CHANGE);
            if(key == MK_SW1)
            {
                delay_up++;
                if(delay_up >= 200)
                {
                    delay_up = 0;
                    if(edit < 2)
                    {
                        edit++;
                    }
                    else
                    {
                        new->hour = hour;
                        new->minute = minute;
//                        new->second = second;
//                        new->second = 0;
                        break;
                    }
                }
            }
            else
            {
                if(delay_up > 1 && delay_up<200)
                {
                    if(edit == 0)
                    {
                        if(hour<23)
                        {
                            hour++;
                        }
                    }
                    else if(edit == 1)
                    {
                        if(minute < 59)
                        {
                            minute++;
                        }
                    }
//                    else if(edit == 2)
//                    {
//                        if(second < 59)
//                        {
//                            second++;
//                        }
//                    }
                }
                delay_up = 0;
            }

            if(key == MK_SW2)
            {
                delay_down++;
                if(delay_down >= 200)
                {
                    delay_down = 0;
                    free(new);
                    return NULL;
                }
            }
            else
            {
                if(delay_down > 1 && delay_down<200)
                {
                    if(edit == 0)
                    {
                        if(hour>0)
                        {
                            hour--;
                        }
                    }
                    else if(edit == 1)
                    {
                        if(minute > 0)
                        {
                            minute--;
                        }
                    }
//                    else if(edit == 2)
//                    {
//                        if(second > 0)
//                        {
//                            second--;
//                        }
//                    }
                }
                delay_down = 0;
            }
            
//            Alarm *new = (Alarm *)malloc(sizeof(Alarm));
            update_time[0] = (hour / 10) + '0';
            update_time[1] = (hour % 10) + '0';
            update_time[2] = ':';
            update_time[3] = (minute / 10) + '0';
            update_time[4] = (minute % 10) + '0';
            update_time[5] = '\0';
//            update_time[6] = (second / 10) + '0';
//            update_time[7] = (second % 10) + '0';
//            update_time[8] = '\0';
            clcd_print(update_time,LINE2(2));
    }
    CLEAR_DISP_SCREEN;
    clcd_print("   SET DATE     ",LINE1(0));
    edit = 0;
    unsigned char day=0,month=0;
    unsigned int year=0;
    day = ((date[0] - '0') * 10) + (date[1] - '0');
    month = ((date[3] - '0') * 10) + (date[4] - '0');
    year = ((date[6] - '0') * 1000) + ((date[7] - '0') * 100) + ((date[8] - '0') * 10) + (date[9] - '0');
    unsigned char update_date[11];
    update_date[0] = (day / 10) + '0';
    update_date[1] = (day % 10) + '0';
    update_date[2] = '-';
    update_date[3] = (month / 10) + '0';
    update_date[4] = (month % 10) + '0';
    update_date[5] = '-';
    update_date[6] = (year / 1000) + '0';
    update_date[7] = ((year / 100) % 10) + '0';
    update_date[8] = ((year / 10) % 10) + '0';
    update_date[9] = (year % 10) + '0';
    update_date[10] = '\0';
    
    while(1)
    {
        unsigned char key = read_switches(LEVEL_CHANGE);
            if(key == MK_SW1)
            {
                delay_up++;
                if(delay_up >= 200)
                {
                    delay_up = 0;
                    if(edit < 3)
                    {
                        edit++;
                    }
                    else
                    {
                        new->day = day;
                        new->month = month;
                        new->year = year;
                        new->next = NULL;
                        new->prev = NULL;
                        new->type = alarm_type;
                        new->id = next_alarm_id++;
                        return new;
                    }
                }
            }
            else
            {
                if(delay_up > 1 && delay_up<200)
                {
                    if(edit == 0)
                    {
                        if(day<31)
                        {
                            day++;
                        }
                    }
                    else if(edit == 1)
                    {
                        if(month < 12)
                        {
                            month++;
                        }
                    }
                    else if(edit == 2)
                    {
                        if(year < 2050)
                        {
                            year++;
                        }
                    }
                }
                delay_up = 0;
            }

            if(key == MK_SW2)
            {
                delay_down++;
                if(delay_down >= 200)
                {
                    delay_down = 0;
                    free(new);
                    return NULL;
                }
            }
            else
            {
                if(delay_down > 1 && delay_down<200)
                {
                    if(edit == 0)
                    {
                        if(day>1)
                        {
                            day--;
                        }
                    }
                    else if(edit == 1)
                    {
                        if(month > 1)
                        {
                            month--;
                        }
                    }
                    else if(edit == 2)
                    {
                        if(year > 2026)
                        {
                            year--;
                        }
                    }
                }
                delay_down = 0;
            }
            
//            Alarm *new = (Alarm *)malloc(sizeof(Alarm));
        update_date[0] = (day / 10) + '0';
        update_date[1] = (day % 10) + '0';
        update_date[2] = '-';
        update_date[3] = (month / 10) + '0';
        update_date[4] = (month % 10) + '0';
        update_date[5] = '-';
        update_date[6] = (year / 1000) + '0';
        update_date[7] = ((year / 100) % 10) + '0';
        update_date[8] = ((year / 10) % 10) + '0';
        update_date[9] = (year % 10) + '0';
        update_date[10] = '\0';
        clcd_print(update_date,LINE2(0));
}
}