#include <avr/pgmspace.h>
#include "io.h"

void io_init(void)
{
	PORTB	= 0x38;
	DDRB	= 0xc7;

	PORTC	= 0xf7;
	DDRC	= 0x00;

	PORTD	= 0x00;
	DDRD	= 0xff;
}

/* ca: 0 = leftmost, 1 = middle, 2 = rightmost, 3 = off */
void io_set_ca(uint8_t ca)
{
	uint8_t pb = PORTB & ~_BV(0);
	uint8_t pd = PORTD & ~(_BV(2) | _BV(1));

	switch(ca) {
		case 0:
			pb |= _BV(0);
			break;

		case 1:
			pd |= _BV(2);
			break;

		case 2:
			pd |= _BV(1);
			break;

		default:
			break;
	}

	PORTB = pb;
	PORTD = pd;
}

void io_set_segments(uint8_t segments)
{
	/* bit 7: port d if unset, port b if set
	 * bit 2...0: bit number (not mask)
	 */

	static const uint8_t map[] PROGMEM = { 0x07, 0x00, 0x04, 0x87, 0x05, 0x06, 0x03, 0x86 };
	uint8_t pb = PORTB;
	uint8_t pd = PORTD;
	uint8_t i;

	for(i = 0; i < 8; ++i) {
		const uint8_t map_entry = pgm_read_byte_near(map + i);
		const uint8_t mask = _BV(map_entry & 0x07);
		uint8_t *port = (map_entry & 0x80) ? &pb : &pd;

		if(segments & _BV(i)) {
			*port &= ~mask;
		}
		else {
			*port |= mask;
		}
	}

	PORTB = pb;
	PORTD = pd;
}
