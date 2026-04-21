#include <cat/util/chrono.hpp>

using namespace cat;
using namespace std::chrono;

millis_t chrono::s_last_millis = 0;
millis_t chrono::s_epoch = 0;
seconds_t chrono::s_delta = 0.f;

static inline millis_t _get_now_millis()
{
	return static_cast<millis_t>(
		duration_cast<milliseconds>(
			system_clock::now().time_since_epoch()
		).count()
	);
}

void chrono::init()
{
	s_epoch = _get_now_millis();
}

void chrono::update()
{
	millis_t now = _get_now_millis();
	millis_t ms_delta = now - s_last_millis;

	s_delta = static_cast<seconds_t>(ms_delta) / 1000.f;
	s_last_millis = now;
}

millis_t chrono::current_millis()
{
	return _get_now_millis() - s_epoch;
}

seconds_t chrono::current_seconds()
{
	return static_cast<seconds_t>(_get_now_millis() - s_epoch) / 1000.f;
}

seconds_t chrono::get_delta()
{
	return s_delta;
}
