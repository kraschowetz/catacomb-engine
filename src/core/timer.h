#ifndef TIMER_H
#define TIMER_H

#include "../util/util.h"

typedef struct {
	u32 start_time;
} Timer;

Timer timer_start();
u32 timer_get_ticks(const Timer *self);
f32 timer_get_time(const Timer *self);

#endif
