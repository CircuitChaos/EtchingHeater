#include <util/atomic.h>
#include "dpy.h"
#include "io.h"

static struct {
	uint8_t cur[3];
	uint8_t pend[3];
	uint8_t phase;
	bool blink;
	uint8_t blink_cnt;
} ctx;

void dpy_init(void)
{
	ctx.cur[0] = ctx.cur[1] = ctx.cur[2] = 0xff;
	ctx.pend[0] = ctx.pend[1] = ctx.pend[2] = 0xff;

	ctx.phase	= 0;
	ctx.blink	= false;
	ctx.blink_cnt	= 0;
}

void dpy_set(const uint8_t seg[3])
{
	ATOMIC_BLOCK(ATOMIC_FORCEON) {
		ctx.pend[0] = seg[0];
		ctx.pend[1] = seg[1];
		ctx.pend[2] = seg[2];
	}
}

void dpy_blink(bool en)
{
	ATOMIC_BLOCK(ATOMIC_FORCEON) {
		ctx.blink = en;
		ctx.blink_cnt = 0;
	}
}

void dpy_tick(void)
{
	io_set_ca(3);
	io_set_segments(ctx.cur[ctx.phase]);
	io_set_ca(ctx.phase);

	if(++ctx.phase == 3) {
		ctx.phase = 0;
		ctx.cur[0] = ctx.pend[0];
		ctx.cur[1] = ctx.pend[1];
		ctx.cur[2] = ctx.pend[2];

		if(ctx.blink) {
			if(++ctx.blink_cnt == 80) {
				ctx.blink_cnt = 0;
			}

			if(ctx.blink_cnt > 40) {
				ctx.cur[0] = ctx.cur[1] = ctx.cur[2] = 0;
			}
		}
	}
}
