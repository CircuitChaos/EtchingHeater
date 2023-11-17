#pragma once

#include <avr/io.h>
#include <inttypes.h>

/* ADC3 */
#define IO_ADMUX	(_BV(MUX1) | _BV(MUX0))

void io_init(void);

/* ca: 0 = leftmost, 1 = middle, 2 = rightmost, 3 = off */
void io_set_ca(uint8_t ca);

/* bit 1 means segment is to be lit (inverted value output because it's CA) */
void io_set_segments(uint8_t segments);

static inline void io_set_k(void) { PORTB |= _BV(2); }
static inline void io_clr_k(void) { PORTB &= ~_BV(2); }
static inline void io_set_buz(void) { PORTB |= _BV(1); }
static inline void io_clr_buz(void) { PORTB &= ~_BV(1); }
