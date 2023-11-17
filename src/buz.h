#pragma once

typedef enum {
	BUZ_NONE,
	BUZ_TEST,
	BUZ_TEMP_LOW,
	BUZ_TEMP_HIGH,
	BUZ_READY,
} buz_mode_t;

void buz_init(void);
void buz_play(buz_mode_t mode);
void buz_tick(void);
