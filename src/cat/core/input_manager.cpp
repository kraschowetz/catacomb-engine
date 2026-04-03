#include "cat/config.hpp"
#include "cat/core/input.hpp"
#include <SDL2/SDL_keycode.h>
#include <cat/core/input_manager.hpp>

#include <cat/util/util.hpp>
#include <SDL2/SDL_events.h>

namespace cat
{

static eKeyType cast_sdl_event_to_cat(SDL_Event& e)
{
	switch(e.key.keysym.sym)
	{
		case SDLK_a: return eKeyType::A;
		case SDLK_b: return eKeyType::B;
		case SDLK_c: return eKeyType::C;
		case SDLK_d: return eKeyType::D;
		case SDLK_e: return eKeyType::E;
		case SDLK_f: return eKeyType::F;
		case SDLK_g: return eKeyType::G;
		case SDLK_h: return eKeyType::H;
		case SDLK_i: return eKeyType::I;
		case SDLK_j: return eKeyType::J;
		case SDLK_k: return eKeyType::K;
		case SDLK_l: return eKeyType::L;
		case SDLK_m: return eKeyType::M;
		case SDLK_n: return eKeyType::N;
		case SDLK_o: return eKeyType::O;
		case SDLK_p: return eKeyType::P;
		case SDLK_q: return eKeyType::Q;
		case SDLK_r: return eKeyType::R;
		case SDLK_s: return eKeyType::S;
		case SDLK_t: return eKeyType::T;
		case SDLK_u: return eKeyType::U;
		case SDLK_v: return eKeyType::V;
		case SDLK_w: return eKeyType::W;
		case SDLK_x: return eKeyType::X;
		case SDLK_y: return eKeyType::Y;
		case SDLK_z: return eKeyType::Z;

		case SDLK_1: return eKeyType::ONE;
		case SDLK_2: return eKeyType::TWO;
		case SDLK_3: return eKeyType::THREE;
		case SDLK_4: return eKeyType::FOUR;
		case SDLK_5: return eKeyType::FIVE;
		case SDLK_6: return eKeyType::SIX;
		case SDLK_7: return eKeyType::SEVEN;
		case SDLK_8: return eKeyType::EIGHT;
		case SDLK_9: return eKeyType::NINE;
		case SDLK_0: return eKeyType::ZERO;

		case SDLK_SPACE: return eKeyType::SPACE;
		case SDLK_LALT: return eKeyType::L_ALT;
		case SDLK_RALT: return eKeyType::R_ALT;
		case SDLK_LCTRL: return eKeyType::L_CTRL;
		case SDLK_RCTRL: return eKeyType::R_CTRL;
		case SDLK_LSHIFT: return eKeyType::L_SHIFT;
		case SDLK_RSHIFT: return eKeyType::R_SHIFT;
		case SDLK_CAPSLOCK: return eKeyType::CAPS;
		case SDLK_TAB: return eKeyType::TAB;
		case SDLK_QUOTE: return eKeyType::QUOTE;
		case SDLK_ESCAPE: return eKeyType::ESC;
		case SDLK_BACKSPACE: return eKeyType::BACKSPACE;
		case SDLK_MINUS: return eKeyType::MINUS;
		case SDLK_PLUS: return eKeyType::PLUS;
		case SDLK_UP: return eKeyType::UP;
		case SDLK_DOWN: return eKeyType::DOWN;
		case SDLK_LEFT: return eKeyType::LEFT;
		case SDLK_RIGHT: return eKeyType::RIGHT;
		case SDLK_F1: return eKeyType::F1;
		case SDLK_F2: return eKeyType::F2;
		case SDLK_F3: return eKeyType::F3;
		case SDLK_F4: return eKeyType::F4;
		case SDLK_F5: return eKeyType::F5;
		case SDLK_F6: return eKeyType::F6;
		case SDLK_F7: return eKeyType::F7;
		case SDLK_F8: return eKeyType::F8;
		case SDLK_F9: return eKeyType::F9;
		case SDLK_F10: return eKeyType::F10;
		case SDLK_F11: return eKeyType::F11;
		case SDLK_F12: return eKeyType::F12;

		default: return eKeyType::NONE;
	}
}

Unique<Input> Input::s_singleton = nullptr;

void Input::update()
{
	SDL_Event event;

	memcpy(
		s_singleton->m_prev_key_buff.data(),
		s_singleton->m_key_buff.data(),
		s_NUM_KEYS
	);
	memcpy(
		s_singleton->m_prev_mouse_buff.data(),
		s_singleton->m_prev_mouse_buff.data(),
		s_NUM_MOUSE_BUTTONS
	);

	while(SDL_PollEvent(&event))
	{
		if(event.type == SDL_KEYDOWN)
		{
			eKeyType kt = cast_sdl_event_to_cat(event);
			u32 id = enum_val(kt);

			s_singleton->m_key_buff[id] = 1;
		}
		else if(event.type == SDL_KEYUP)
		{
			eKeyType kt = cast_sdl_event_to_cat(event);
			u32 id = enum_val(kt);
			
			s_singleton->m_key_buff[id] = 0;
		}
		else if(event.type == SDL_QUIT || event.type == SDL_APP_TERMINATING)
		{
			s_singleton->m_is_queued_system_exit = true;
		}
	}
}

void Input::init()
{
	s_singleton = std::make_unique<Input>();

	s_singleton->m_prev_key_buff.assign(s_NUM_KEYS, 0);
	s_singleton->m_key_buff.assign(s_NUM_KEYS, 0);
	s_singleton->m_prev_mouse_buff.assign(s_NUM_MOUSE_BUTTONS, 0);
	s_singleton->m_mouse_buff.assign(s_NUM_MOUSE_BUTTONS, 0);
}

void Input::quit()
{
	s_singleton.reset();
}

bool Input::is_key_pressed(eKeyType key)
{
	u32 id = enum_val(key);

	return s_singleton->m_key_buff[id];
}

bool Input::is_key_just_pressed(eKeyType key)
{
	u32 id = enum_val(key);

	return s_singleton->m_key_buff[id] && !s_singleton->m_prev_key_buff[id];
}

bool Input::is_key_just_released(eKeyType key)
{
	u32 id = enum_val(key);

	return s_singleton->m_prev_key_buff[id] && !s_singleton->m_key_buff[id];
}

bool Input::is_mouse_button_pressed(eMouseButton button)
{
	u32 id = enum_val(button);
	
	return s_singleton->m_mouse_buff[id];
}

bool Input::is_mouse_button_just_pressed(eMouseButton button)
{
	u32 id = enum_val(button);

	return s_singleton->m_mouse_buff[id] && !s_singleton->m_prev_mouse_buff[id];
}

bool Input::is_mouse_button_just_released(eMouseButton button)
{
	u32 id = enum_val(button);

	return !s_singleton->m_mouse_buff[id] && s_singleton->m_prev_mouse_buff[id];
}

bool Input::has_queued_exit()
{
	return s_singleton->m_is_queued_system_exit;
}

}
