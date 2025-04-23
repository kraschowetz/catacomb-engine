#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL_events.h>
#include "../util/util.h"

#define KEY_BUFFER_SIZE 256
#define BUTTON_BUFFER_SIZE 9

/* extern SDL_Joystick *joystick; */

void update_input_state(SDL_Event e);

// maybe use u32 in get_keys to match SDL key types
bool get_key_pressed(u8 key);
bool get_key_just_pressed(u8 key);
bool get_key_just_released(u8 key);

bool get_mouse_button_pressed(u8 button);
bool get_mouse_button_just_pressed(u8 button);
bool get_mouse_button_just_released(u8 button);
u8 get_mouse_click_ammount();
//TODO: vec2f get_mouse_position();

#endif
