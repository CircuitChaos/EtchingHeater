#pragma once

#include <stdbool.h>
#include <inttypes.h>

void filter_init(void);

/* ADC samples: 8000000/128/13 = 4808 Hz
 * Oversampled samples: / 1024 = 4.7 Hz
 * Moving average: / 16 = 0.29 Hz
 *
 * Input range: 0...1023
 * Output range: 0...65472
 */

bool filter_run(uint16_t *out, uint16_t in);
