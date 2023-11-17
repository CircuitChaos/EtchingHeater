#pragma once

#include <stdbool.h>

void htr_on(void);
void htr_off(void);
bool htr_is_on(void);

/* After heater has been switched, we disallow ADC for some time (8 tries) */
bool htr_adc_allowed(void);
