#include <avr/interrupt.h>
#include "tmr.h"
#include "dpy.h"
#include "buz.h"

/* Timer ticks at 8 MHz / 64 / 256 = 488 Hz (2048 us) */
ISR(TIMER2_OVF_vect)
{
	static uint8_t buz_div = 0;

	dpy_tick();

	if(++buz_div == (488 / 16)) {
		buz_div = 0;
		buz_tick();
	}
}

void tmr_init(void)
{
	TCCR2	= _BV(CS22);
	TCNT2	= 0;
	TIMSK	|= _BV(TOIE2);
}
