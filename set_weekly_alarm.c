//#include "ds1307.h"
//#include "i2c.h"
//#include "main.h"
//#include "clcd.h"
//#include "matrix_keypad.h"
//#include "external_eeprom.h"
//#include "alarm.h"
//
//Alarm *set_weekly_alarm(unsigned char alarm_type)
//{
//    unsigned int delay_up = 0;
//    unsigned int delay_down = 0;
//    CLEAR_DISP_SCREEN;
//    unsigned char hour,minute,second;
//    hour = ((time[0] - '0') * 10) + (time[1] - '0');
//    minute = ((time[3] - '0') * 10) + (time[4] - '0');
//    second = ((time[6] - '0') * 10) + (time[7] - '0');
//    clcd_print(" SET DAILY ALARM",LINE1(0));
//    unsigned char update_time[9];
//    update_time[0] = (hour / 10) + '0';
//    update_time[1] = (hour % 10) + '0';
//    update_time[2] = ':';
//    update_time[3] = (minute / 10) + '0';
//    update_time[4] = (minute % 10) + '0';
//    update_time[5] = ':';
//    update_time[6] = (second / 10) + '0';
//    update_time[7] = (second % 10) + '0';
//    update_time[8] = '\0';
//    unsigned char edit = 0;
//    
//    while(1)
//    {    
//        if(alarm_type == 'O')
//        {
//            unsigned char key = read_switches(LEVEL_CHANGE);
//            if(key == MK_SW1)
//            {
//                delay_up++;
//                if(delay_up >= 200)
//                {
//                    delay_up = 0;
//                    if(edit < 3)
//                    {
//                        edit++;
//                    }
//                    else
//                    {
//                        Alarm *new = (Alarm *)malloc(sizeof(Alarm));
//                    }
//                }
//            }
//            else
//            {
//                delay_up = 0;
//            }
//
//            if(key == MK_SW2)
//            {
//                delay_down++;
//                if(delay_down >= 200)
//                {
//                    delay_down = 0;
//                    return 0;
//                }
//            }
//            else
//            {
//                delay_down = 0;
//            }
//            
////            Alarm *new = (Alarm *)malloc(sizeof(Alarm));
//            
//            if(key == MK_SW1 && delay_up > 1 && delay_up<200)
//            {
//                if(edit == 0)
//                {
//                    if(hour<12)
//                    {
//                        hour++;
//                    }
//                }
//                else if(edit == 1)
//                {
//                    if(minute < 59)
//                    {
//                        minute++;
//                    }
//                }
//                else if(edit == 2)
//                {
//                    if(second < 59)
//                    {
//                        second++;
//                    }
//                }
//            }
//            if(key == MK_SW2 && delay_down > 1 && delay_down<200)
//            {
//                if(edit == 0)
//                {
//                    if(hour>1)
//                    {
//                        hour--;
//                    }
//                }
//                else if(edit == 1)
//                {
//                    if(minute > 0)
//                    {
//                        minute--;
//                    }
//                }
//                else if(edit == 2)
//                {
//                    if(second > 0)
//                    {
//                        second--;
//                    }
//                }
//            }
//            update_time[0] = (hour / 10) + '0';
//            update_time[1] = (hour % 10) + '0';
//            update_time[2] = ':';
//            update_time[3] = (minute / 10) + '0';
//            update_time[4] = (minute % 10) + '0';
//            update_time[5] = ':';
//            update_time[6] = (second / 10) + '0';
//            update_time[7] = (second % 10) + '0';
//            update_time[8] = '\0';
//            clcd_print(update_time,LINE2(2));
//            
//        }
//        else if(alarm_type == 'D')
//        {
//
//        }
//        else if(alarm_type == 'W')
//        {
//
//        }
//    }
//}