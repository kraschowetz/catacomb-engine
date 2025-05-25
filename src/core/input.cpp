#include "input.hpp"

static bool _key_buffer[KEY_BUFFER_SIZE] = {0};
static bool _prev_key_buffer[KEY_BUFFER_SIZE] = {0};
static bool _button_buffer[BUTTON_BUFFER_SIZE] = {0};
static bool _prev_button_buffer[BUTTON_BUFFER_SIZE] = {0};

static u8 _mouse_clicks = 0;

void input::update(SDL_Event e) {
	memcpy(_prev_key_buffer, _key_buffer, sizeof(bool) * KEY_BUFFER_SIZE);
	memcpy(_prev_button_buffer, _button_buffer, sizeof(bool) * BUTTON_BUFFER_SIZE);
	_mouse_clicks = 0;
	
	switch(e.type) {
		case SDL_KEYDOWN:
			_key_buffer[(u8)e.key.keysym.sym] = 1;
			break;
		case SDL_KEYUP:
			_key_buffer[(u8)e.key.keysym.sym] = 0;
			break;
		case SDL_MOUSEBUTTONDOWN:
			_button_buffer[e.button.button] = 1;
			_mouse_clicks = e.button.clicks;
			break;
		case SDL_MOUSEBUTTONUP:
			_button_buffer[e.button.button] = 0;
			break;

	}
}

bool input::get_key_pressed(u8 key) {
	return _key_buffer[key];
}

bool input::get_key_just_pressed(u8 key) {
	return _key_buffer[key] && !_prev_key_buffer[key];
}

bool input::get_key_just_released(u8 key) {
	return _prev_key_buffer[key] && !_key_buffer[key];
}

bool input::get_mouse_button_pressed(u8 button) {
	return _button_buffer[button];
}

bool input::get_mouse_button_just_pressed(u8 button) {
	return _button_buffer[button] && !_prev_button_buffer[button];
}

bool input::get_mouse_button_just_released(u8 button) {
	return _prev_button_buffer[button] && !_button_buffer[button];
}

u8 input::get_mouse_click_ammount() {
	return _mouse_clicks;
}

i8 input::get_key_axis(u8 negative, u8 positive) {
	i8 res = 0;
	if(_button_buffer[negative])
		res--;
	if(_button_buffer[positive])
		res++;
	return res;
}
