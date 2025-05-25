#ifndef INPUT_H
#define INPUT_H

#include "../util/util.hpp"
#include <SDL2/SDL_events.h>

#define KEY_BUFFER_SIZE 256
#define BUTTON_BUFFER_SIZE 9

namespace input {
	void update(SDL_Event e);
	
	bool get_key_pressed(u8 key);
	bool get_key_just_pressed(u8 key);
	bool get_key_just_released(u8 key);

	bool get_mouse_button_pressed(u8 button);
	bool get_mouse_button_just_pressed(u8 button);
	bool get_mouse_button_just_released(u8 button);
	u8 get_mouse_click_ammount();

	i8 get_key_axis(u8 negative, u8 positive);
}

#endif // INPUT_H
