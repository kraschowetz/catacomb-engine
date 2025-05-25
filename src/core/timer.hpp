#ifndef TIMER_H
#define TIMER_H

#include "../util/util.hpp"

class Timer {
private:
	u32 start_ticks;
public:
	void init();
	u32 get_ticks();
	f32 get_time();
};

#endif // TIMER_H
