#include "ds1307.h"
#include "i2c.h"
#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "external_eeprom.h"
#include "alarm.h"

Alarm *find_nearest_alarm(void)
{
    get_time();
    get_date();

    unsigned char hour = ((time[0] - '0') * 10) + (time[1] - '0');
    unsigned char minute = ((time[3] - '0') * 10) + (time[4] - '0');

    unsigned char day = ((date[0] - '0') * 10) + (date[1] - '0');
    unsigned char month = ((date[3] - '0') * 10) + (date[4] - '0');
    unsigned int year = ((date[6] - '0') * 1000) +
                        ((date[7] - '0') * 100) +
                        ((date[8] - '0') * 10) +
                        (date[9] - '0');

    /* Delete expired once alarms */
    while(head_once != NULL)
    {
        if((head_once->year < year) ||
           (head_once->year == year && head_once->month < month) ||
           (head_once->year == year && head_once->month == month && head_once->day < day) ||
           (head_once->year == year && head_once->month == month &&
            head_once->day == day && head_once->hour < hour) ||
           (head_once->year == year && head_once->month == month &&
            head_once->day == day && head_once->hour == hour &&
            head_once->minute < minute))
        {
            Alarm *temp = head_once;
            head_once = head_once->next;

            if(head_once)
                head_once->prev = NULL;

            free(temp);
        }
        else
        {
            break;
        }
    }

    Alarm *once_alarm = head_once;

    /* Find today's next daily alarm */
    Alarm *today_daily = head_daily;

    while(today_daily)
    {
        if(today_daily->hour > hour)
            break;

        if(today_daily->hour == hour &&
           today_daily->minute >= minute)
            break;

        today_daily = today_daily->next;
    }

    /* No alarms at all */
    if(once_alarm == NULL && head_daily == NULL)
        return NULL;

    /* Only daily alarms */
    if(once_alarm == NULL)
    {
        if(today_daily)
            return today_daily;
        else
            return head_daily;
    }

    /* Only once alarms */
    if(head_daily == NULL)
        return once_alarm;

    /* ---------- ONCE ALARM IS TODAY ---------- */
    if(once_alarm->year == year &&
       once_alarm->month == month &&
       once_alarm->day == day)
    {
        /* No daily left today */
        if(today_daily == NULL)
            return once_alarm;

        if(once_alarm->hour < today_daily->hour)
            return once_alarm;

        if(once_alarm->hour > today_daily->hour)
            return today_daily;

        if(once_alarm->minute < today_daily->minute)
            return once_alarm;

        return today_daily;
    }

    /* ---------- ONCE ALARM IS FUTURE ---------- */

    /* If a daily alarm still exists today,
       it'll always occur before a future once alarm */
    if(today_daily != NULL)
        return today_daily;

    /* Otherwise tomorrow's first daily alarm occurs
       before any once alarm after tomorrow */
    return head_daily;
}