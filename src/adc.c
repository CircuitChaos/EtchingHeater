#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include "adc.h"
#include "io.h"

static volatile uint16_t adc_result = UINT16_MAX;

ISR(ADC_vect)
{
	adc_result = ADC;
}

void adc_init(void)
{
	ADCSRA = _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
	ADMUX = IO_ADMUX;
}

uint16_t adc_read(void)
{
	ADCSRA |= _BV(ADEN);
	adc_result = UINT16_MAX;
	set_sleep_mode(SLEEP_MODE_ADC);

	while(adc_result == UINT16_MAX) {
		sleep_mode();
	}

	ADCSRA &= ~_BV(ADEN);
	return adc_result;
}
