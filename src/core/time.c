#include "time.h"
#include "timer.h"
#include <stdio.h>

f32 _last_time = 0;

Time global_time = {
	.delta_time = 0.f,
	.program_time = 0.f,
	.frames_per_second = 0.f,
	.delta_timer = {},
	.program_timer = {}
};

void init_global_time() {
	global_time.delta_timer = timer_start();
	global_time.program_timer = timer_start();
}

void update_global_time() {
	global_time.frames_per_second++;
	global_time.delta_time = timer_get_time(&global_time.delta_timer);
	global_time.delta_timer = timer_start();
	global_time.program_time = timer_get_time(&global_time.program_timer);

	if(global_time.program_time > _last_time) {
		_last_time += 1.f;
		printf("FPS: %u\n", global_time.frames_per_second);
		global_time.frames_per_second = 0;
	}
}
