#include <util/atomic.h>
#include "io.h"
#include "htr.h"

static bool state = false;
static uint8_t cnt = 0;

void htr_on(void)
{
	if(!state) {
		ATOMIC_BLOCK(ATOMIC_FORCEON) {
			io_set_k();
		}
		state = true;
		cnt = 8;
	}
}

void htr_off(void)
{
	if(state) {
		ATOMIC_BLOCK(ATOMIC_FORCEON) {
			io_clr_k();
		}
		state = false;
		cnt = 8;
	}
}

bool htr_is_on(void)
{
	return state;
}

bool htr_adc_allowed(void)
{
	if(!cnt) {
		return true;
	}

	--cnt;
	return false;
}
