#pragma once

#include <inttypes.h>
#include <stdbool.h>

void dpy_init(void);
void dpy_set(const uint8_t seg[3]);
void dpy_blink(bool en);
void dpy_tick(void);
