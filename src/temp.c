#include "temp.h"

#define UREF		2472
#define TEMP_HYST_HI	500
#define TEMP_HYST_LO	490
#define TEMP_LO		150
#define TEMP_HI		550
#define TEMP_MAX	(UREF * 0.9)
#define TEMP_READY	400

uint16_t temp_from_filter(uint16_t filtered)
{
	/* - ADC after oversampling and filtering: 16-bit (65536)
	 *
	 * Reference voltage measured: stored in UREF
	 * Input voltage: temperature * 10 mV/C
	 *
	 * temp [C] = uref * filt / (10 * 65536)
	 * temp [C * 10] = uref * filt / 65536
	 */

	return (uint32_t) UREF * filtered / 65536;
}

bool temp_too_low(uint16_t temp)
{
	return temp <= TEMP_LO;
}

bool temp_too_high(uint16_t temp)
{
	return temp >= TEMP_HI;
}

bool temp_below_thres(uint16_t temp)
{
	return temp <= TEMP_HYST_LO;
}

bool temp_above_thres(uint16_t temp)
{
	return temp >= TEMP_HYST_HI;
}

bool temp_is_max(uint16_t temp)
{
	return temp >= TEMP_MAX;
}

bool temp_is_ready(uint16_t temp)
{
	return temp >= TEMP_READY;
}
