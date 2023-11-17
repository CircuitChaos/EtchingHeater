#include <avr/pgmspace.h>
#include "7seg.h"

uint8_t sevenseg(uint8_t digit)
{
	static const uint8_t map[] PROGMEM = {
		0x3f, 0x06, 0x5b, 0x4f, 0x66,
		0x6d, 0x7d, 0x07, 0x7f, 0x6f,
	};

	if(digit > 9) {
		return 0;
	}

	return pgm_read_byte_near(map + digit);
}
