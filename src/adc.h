#pragma once

#include <inttypes.h>

void adc_init(void);

/* To be triggered by the timer, but called from main. Enters ADC sleep for
 * approx. 128 * 25 = 3200 cycles, or 1000000 / (8 MHz / 128 / 25) = 400 us
 *
 * Note that:
 * - interrupts must be on before calling this
 * - sleep mode will be reconfigured after this returns
 */

uint16_t adc_read(void);
