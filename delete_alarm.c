#include "alarm.h"
#include <stdlib.h>

void delete_alarm(Alarm *current)
{
    if(current == NULL)
        return;

    /* ONCE ALARM LIST */
    if(current->type == 'O')
    {
        if(current->prev == NULL)
            head_once = current->next;
        else
            current->prev->next = current->next;

        if(current->next != NULL)
            current->next->prev = current->prev;

        once_count--;
    }

    /* DAILY ALARM LIST */
    else if(current->type == 'D')
    {
        if(current->prev == NULL)
            head_daily = current->next;
        else
            current->prev->next = current->next;

        if(current->next != NULL)
            current->next->prev = current->prev;

        daily_count--;
    }

    /* WEEKLY ALARM LIST */
    else if(current->type == 'W')
    {
        if(current->prev == NULL)
            head_weekly = current->next;
        else
            current->prev->next = current->next;

        if(current->next != NULL)
            current->next->prev = current->prev;

        weekly_count--;
    }

    free(current);
}