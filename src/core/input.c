#include "input.h"
#include <SDL2/SDL_joystick.h>

/* SDL_Joystick *joystick = NULL; */

/*
 * add joystick support?
 */

static bool _key_buffer[KEY_BUFFER_SIZE] = {0};
static bool _prev_key_buffer[KEY_BUFFER_SIZE] = {0};
static bool _button_buffer[BUTTON_BUFFER_SIZE] = {0};
static bool _prev_button_buffer[BUTTON_BUFFER_SIZE] = {0};

u8 _mouse_clicks = 0;

void update_input_state(SDL_Event e) {
	memcpy(_prev_key_buffer, _key_buffer, sizeof(bool) * KEY_BUFFER_SIZE);
	memcpy(_prev_button_buffer, _button_buffer, sizeof(bool) * BUTTON_BUFFER_SIZE);
	_mouse_clicks = 0;
	
	switch(e.type) {
		case SDL_KEYDOWN:
			_key_buffer[(u8)e.key.keysym.sym] = 1;
			break;
		case SDL_KEYUP:
			_key_buffer[(u8)e.key.keysym.sym] = 0;
			printf("%d\n", SDL_NumJoysticks());
			break;
		case SDL_MOUSEBUTTONDOWN:
			_button_buffer[e.button.button] = 1;
			_mouse_clicks = e.button.clicks;
			break;
		case SDL_MOUSEBUTTONUP:
			_button_buffer[e.button.button] = 0;
			break;
		/* case SDL_JOYBUTTONDOWN: */
		/* 	printf("%d button down\n", e.jbutton.type); */
		/* 	break; */
		/* case SDL_JOYBUTTONUP: */
		/* 	printf("%d button up\n", e.jbutton.type); */
		/* 	break; */
		/* case SDL_JOYHATMOTION: */
		/* 	printf("%d hat motion\n", e.jhat.value); */
		/* 	// hat always override the last axys value */
		/* 	break; */
		/* case SDL_JOYAXISMOTION: */
		/* 	printf("axys %d with value %f\n", e.jaxis.axis, e.jaxis.value / 32767.f); */
		/* 	// axys always override the last axys value */
		/* 	break; */

	}
}

bool get_key_pressed(u8 key) {
	return _key_buffer[key];
}

bool get_key_just_pressed(u8 key) {
	return _key_buffer[key] && !_prev_key_buffer[key];
}

bool get_key_just_released(u8 key) {
	return _prev_key_buffer[key] && !_key_buffer[key];
}

bool get_mouse_button_pressed(u8 button) {
	return _button_buffer[button];
}

bool get_mouse_button_just_pressed(u8 button) {
	return _button_buffer[button] && !_prev_button_buffer[button];
}

bool get_mouse_button_just_released(u8 button) {
	return _prev_button_buffer[button] && !_button_buffer[button];
}

u8 get_mouse_click_ammount() {
	return _mouse_clicks;
}
