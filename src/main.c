#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <stdbool.h>
#include "io.h"
#include "adc.h"
#include "tmr.h"
#include "htr.h"
#include "temp.h"
#include "7seg.h"
#include "buz.h"
#include "dpy.h"
#include "filter.h"

static void handle_hysteresis(uint16_t temp)
{
	if(!htr_is_on() && temp_below_thres(temp)) {
		htr_on();
	}
	else if(htr_is_on() && temp_above_thres(temp)) {
		htr_off();
	}
}

static void handle_ready(uint16_t temp)
{
	static bool sounded = false;
	if(sounded) {
		return;
	}

	if(temp_is_ready(temp)) {
		sounded = true;
		buz_play(BUZ_READY);
	}
}

static void fill_segments(uint8_t segments[3], uint16_t temp, bool htr)
{
	if(temp > 999) {
		segments[0] = sevenseg(1);
		segments[1] = 0;
		segments[2] = 0;
	}
	else {
		segments[0] = sevenseg(temp / 100);
		segments[1] = sevenseg((temp / 10) % 10) | 0x80;
		segments[2] = sevenseg(temp % 10);
	}

	if(htr) {
		segments[2] |= 0x80;
	}
}

static void handle_adc(uint16_t adc)
{
	uint16_t filtered;
	uint16_t temp;
	static bool error = false;
	uint8_t segments[3];

	if(!filter_run(&filtered, adc)) {
		return;
	}

	temp = temp_from_filter(filtered);

	/* In case of error, we disable heater and sound until rebooted.
	 * That's intentional. */

	if(!error) {
		if(temp_too_low(temp) || temp_too_high(temp)) {
			htr_off();
			error = true;
			dpy_blink(true);
			buz_play(temp_too_low(temp) ? BUZ_TEMP_LOW : BUZ_TEMP_HIGH);
		}
		else {
			handle_hysteresis(temp);
			handle_ready(temp);
		}
	}

	fill_segments(segments, temp, htr_is_on());
	dpy_set(segments);
}

int main(void)
{
	io_init();
	wdt_enable(WDTO_15MS);
	adc_init();
	filter_init();
	buz_init();
	dpy_init();
	tmr_init();
	sei();

	for(;;) {
		wdt_reset();

		/* Waiting for timer */
		set_sleep_mode(SLEEP_MODE_IDLE);
		sleep_mode();

		/* Another sleep mode, waiting for ADC */
		if(htr_adc_allowed()) {
			handle_adc(adc_read());
		}
	}
}
