#include <avr/pgmspace.h>
#include <util/atomic.h>
#include "buz.h"
#include "io.h"

/* Morse table... inefficient, but we have plenty of flash anyway.
 *
 * - dot: 0=sound, 1=nosound
 * - dash: 0=sound, 1=sound, 2=sound, 3=nosound
 * - space: 0=nosound, 1=nosound
 * - R: repeat
 * - E: end
 */

/* E E */
static const char pat_test[] PROGMEM = ".. E";

/* LOW */
static const char pat_temp_low[] PROGMEM = ".-.. --- .--  R";

/* HIGH */
static const char pat_temp_high[] PROGMEM = ".... .. --. ....  R";

/* READY */
static const char pat_ready[] PROGMEM = ".-. . .- -.. -.--  E";

static struct {
	/* Currently played mode */
	buz_mode_t mode;

	/* Pending mode, set by buz_play */
	buz_mode_t pend;

	/* Address of currently played string */
	uint16_t addr;

	/* Current offset in this string */
	uint8_t ofs;

	/* Phase in current character */
	uint8_t phase;
} ctx;

void buz_init(void)
{
	ctx.mode	= BUZ_NONE;
	ctx.pend	= BUZ_TEST;
	ctx.addr	= 0;
	ctx.ofs		= 0;
	ctx.phase	= 0;
}

void buz_play(buz_mode_t mode)
{
	ATOMIC_BLOCK(ATOMIC_FORCEON) {
		ctx.pend = mode;
	}
}

void buz_tick(void)
{
	char ch;

	if(ctx.pend != ctx.mode) {
		ctx.mode = ctx.pend;

		switch(ctx.mode) {
			case BUZ_NONE:
				io_clr_buz();
				return;

			case BUZ_TEST:
				ctx.addr = (uint16_t) pat_test;
				break;

			case BUZ_TEMP_LOW:
				ctx.addr = (uint16_t) pat_temp_low;
				break;

			case BUZ_TEMP_HIGH:
				ctx.addr = (uint16_t) pat_temp_high;
				break;

			case BUZ_READY:
				ctx.addr = (uint16_t) pat_ready;
				break;

			default:
				break;
		}

		ctx.phase = 0;
		ctx.ofs = 0;
	}

	ch = pgm_read_byte_near(ctx.addr + ctx.ofs);
	switch(ch) {
		case '.':
			if(ctx.phase == 0) {
				io_set_buz();
				ctx.phase = 1;
			}
			else {
				io_clr_buz();
				++ctx.ofs;
				ctx.phase = 0;
			}
			break;

		case '-':
			if(ctx.phase == 0) {
				io_set_buz();
				ctx.phase = 1;
			}
			else if(ctx.phase == 1 || ctx.phase == 2) {
				++ctx.phase;
			}
			else {
				io_clr_buz();
				++ctx.ofs;
				ctx.phase = 0;
			}
			break;

		case ' ':
			if(ctx.phase == 0) {
				++ctx.phase;
			}
			else {
				++ctx.ofs;
				ctx.phase = 0;
			}
			break;

		case 'R':
			ctx.ofs = 0;
			break;

		case 'E':
			ctx.mode = BUZ_NONE;
			ctx.pend = BUZ_NONE;
			break;

		default:
			break;
	}
}
