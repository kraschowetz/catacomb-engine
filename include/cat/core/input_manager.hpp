#pragma once

#include <cat/util/memory.hpp>
#include <cat/core/input_mappings.hpp>
#include <vector>

namespace cat
{

class InputManager
{
public:
    InputManager();

	void update();

	bool is_key_pressed(eKeyType key) const;
	bool is_key_just_pressed(eKeyType key) const;
	bool is_key_just_released(eKeyType key) const;

	bool is_mouse_button_pressed(eMouseButton button) const;
	bool is_mouse_button_just_pressed(eMouseButton button) const;
	bool is_mouse_button_just_released(eMouseButton button) const;

	bool has_queued_exit() const;

private:
	std::vector<u8> m_prev_key_buff;
	std::vector<u8> m_key_buff;
	std::vector<u8> m_prev_mouse_buff;
	std::vector<u8> m_mouse_buff;

	bool m_is_queued_system_exit = false;
	
	static constexpr u32 s_NUM_KEYS = 256;
	static constexpr u32 s_NUM_MOUSE_BUTTONS = 16;
};

}
