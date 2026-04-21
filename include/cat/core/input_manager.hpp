#pragma once

#include <cat/core/memory.hpp>
#include <cat/core/input.hpp>
#include <vector>

namespace cat
{

struct input
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
	input() = default;

	static std::vector<u8> s_prev_key_buff;
	static std::vector<u8> s_key_buff;
	static std::vector<u8> s_prev_mouse_buff;
	static std::vector<u8> s_mouse_buff;

	static bool s_is_queued_system_exit;
	
	static constexpr u32 s_NUM_KEYS = 256;
	static constexpr u32 s_NUM_MOUSE_BUTTONS = 16;
};

}
