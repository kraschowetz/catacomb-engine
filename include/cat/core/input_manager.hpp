#pragma once

#include <cat/core/memory.hpp>
#include <cat/core/input.hpp>
#include <vector>

namespace cat
{

class Input
{
public:
	static void update();

	static void init();
	static void quit();

	static bool is_key_pressed(eKeyType key);
	static bool is_key_just_pressed(eKeyType key);
	static bool is_key_just_released(eKeyType key);

	static bool is_mouse_button_pressed(eMouseButton button);
	static bool is_mouse_button_just_pressed(eMouseButton button);
	static bool is_mouse_button_just_released(eMouseButton button);

	static bool has_queued_exit();

private:
	std::vector<u8> m_prev_key_buff;
	std::vector<u8> m_key_buff;
	std::vector<u8> m_prev_mouse_buff;
	std::vector<u8> m_mouse_buff;

	bool m_is_queued_system_exit = false;

	static Unique<Input> s_singleton;
	
	static constexpr u32 s_NUM_KEYS = 256;
	static constexpr u32 s_NUM_MOUSE_BUTTONS = 16;
};

}
