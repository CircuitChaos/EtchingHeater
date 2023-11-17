#include "filter.h"

#define OVERSAMPLE_CYCLES 256
#define MAVG_CYCLES 16

static struct {
	uint32_t oversample_acc;
	uint16_t oversample_cnt;
	uint32_t mavg_data[MAVG_CYCLES];
	uint8_t mavg_cnt;
} ctx;

void filter_init(void)
{
	ctx.oversample_acc = 0;
	ctx.oversample_cnt = 0;
	ctx.mavg_cnt = 0;
}

bool filter_run(uint16_t *out, uint16_t in)
{
	uint32_t value;
	uint16_t i;
	uint32_t sum;

	ctx.oversample_acc += in;
	++ctx.oversample_cnt;

	if(ctx.oversample_cnt < OVERSAMPLE_CYCLES) {
		return false;
	}

	value = ctx.oversample_acc;
	ctx.oversample_acc = 0;
	ctx.oversample_cnt = 0;

	if(ctx.mavg_cnt < MAVG_CYCLES) {
		ctx.mavg_data[ctx.mavg_cnt++] = value;
		return false;
	}

	for(i = 1; i < MAVG_CYCLES; ++i) {
		ctx.mavg_data[i - 1] = ctx.mavg_data[i];
	}

	ctx.mavg_data[MAVG_CYCLES - 1] = value;
	sum = 0;
	for(i = 0; i < MAVG_CYCLES; ++i) {
		sum += ctx.mavg_data[i];
	}

	*out = sum / MAVG_CYCLES / 4;
	return true;
}
