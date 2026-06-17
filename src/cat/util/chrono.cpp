#include <cat/util/chrono.hpp>

using namespace cat;
using namespace std::chrono;

static inline millis_t _get_now_millis()
{
	return static_cast<millis_t>(
		duration_cast<milliseconds>(
			system_clock::now().time_since_epoch()
		).count()
	);
}

Chrono::Chrono()
{
	m_epoch = _get_now_millis();
    m_last_second = current_millis();
}

void Chrono::update()
{
	millis_t now = _get_now_millis();
	millis_t ms_delta = now - m_last_millis;

    if(now >= m_last_second + 1000)
    {
        m_last_frames = m_current_frames;
        m_current_frames = 0;
        m_last_second = now;
    }

	m_delta = static_cast<seconds_t>(ms_delta) / 1000.f;
	m_last_millis = now;
    m_current_frames++;
}

millis_t Chrono::current_millis() const
{
	return _get_now_millis() - m_epoch;
}

seconds_t Chrono::current_seconds() const
{
	return static_cast<seconds_t>(_get_now_millis() - m_epoch) / 1000.f;
}

seconds_t Chrono::get_delta() const
{
	return m_delta;
}

u16 Chrono::get_fps() const
{
    return m_last_frames;
}
