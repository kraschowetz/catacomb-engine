#ifndef TIME_H
#define TIME_H

#include "timer.h"

typedef struct Time {
	u16 frames_per_second;
	f32 program_time, delta_time;
	Timer program_timer, delta_timer;
} Time;

extern Time global_time;

void init_global_time(void);
void update_global_time(void);

#endif // TIME_H
