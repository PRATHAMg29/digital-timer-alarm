#include "ds1307.h"
#include "i2c.h"
#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "external_eeprom.h"
#include "alarm.h"

Alarm *set_daily_alarm(unsigned char alarm_type)
{
    unsigned int delay_up = 0;
    unsigned int delay_down = 0;
    CLEAR_DISP_SCREEN;
    unsigned char hour,minute;
    hour = ((time[0] - '0') * 10) + (time[1] - '0');
    minute = ((time[3] - '0') * 10) + (time[4] - '0');
//    second = ((time[6] - '0') * 10) + (time[7] - '0');
    clcd_print(" SET DAILY ALARM",LINE1(0));
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
                        new->prev = NULL;
                        new->next = NULL;
                        new->day = 0;
                        new->month = 0;
                        new->year = 0;
                        new->id = next_alarm_id++;
                        new->type = alarm_type;
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
}