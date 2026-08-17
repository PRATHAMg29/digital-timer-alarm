#include <xc.h>
#include "timer.h"

void init_timer0(void)
{
	/*
	 * Setting instruction cycle clock (Fosc / 4) as the source of
	 * timer0
	 */

/*Setting 8 bit timer register*/
	T08BIT = 1;

/* Selecting internal clock source */
	T0CS = 0;

/* Enabling timer0*/
	TMR0ON = 1;

/* Enable prescaler */
    PSA = 0;

/* Prescaler = 1:32 */
    T0PS2 = 1;
    T0PS1 = 0;
    T0PS0 = 0;

	TMR0 = 6;

	/* Clearing timer0 overflow interrupt flag bit */
	TMR0IF = 0;

	/* Enabling timer0 overflow interrupt */
	TMR0IE = 1;
}

