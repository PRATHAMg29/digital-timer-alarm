#include <xc.h>
#include "clcd.h"
#include "ds1307.h"
#include "i2c.h"
#include "main.h"
#include "matrix_keypad.h"
#include "timer.h"
#include "external_eeprom.h"
#include "alarm.h"

unsigned char key_event(unsigned char key, unsigned char sw, KeyStatus *status, unsigned int hold_time)
{
    if(key == sw)
    {
        status->delay++;
        if(status->delay >= hold_time)
        {
            status->delay = 0;
            return 2;       // Long press
        }
    }
    else
    {
        if(status->delay > 1 && status->delay < hold_time)
        {
            status->delay = 0;
            return 1;       // Short press
        }
        status->delay = 0;
    }
    return 0;               // No event
}