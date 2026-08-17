#include <xc.h>
#include "main.h"

void __interrupt() isr(void)
{
    static unsigned short count = 0;
    extern unsigned char seconds;
    extern unsigned char event;
    /* Timer0 */
    if(TMR0IF)
    {
        TMR0 += 8;
        
        if(++count >= 625)
        {
            count = 0;
            seconds++;
        }
        
        if(seconds == 5)
        {
            event = 1;
            seconds = 0;
        }
        
        TMR0IF = 0;
    }
    
}