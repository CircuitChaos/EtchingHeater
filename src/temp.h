#pragma once

#include <inttypes.h>
#include <stdbool.h>

uint16_t temp_from_filter(uint16_t filtered);
bool temp_too_low(uint16_t temp);
bool temp_too_high(uint16_t temp);
bool temp_below_thres(uint16_t temp);
bool temp_above_thres(uint16_t temp);
bool temp_is_max(uint16_t temp);
bool temp_is_ready(uint16_t temp);
