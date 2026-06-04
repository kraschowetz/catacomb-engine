#pragma once

#include "cat/config.hpp"
#include <cfloat>
#include <limits>

namespace cat
{

enum class eKeyType : u32
{
	// letters
	A = 'A',
	B = 'B',
	C = 'C',
	D = 'D',
	E = 'E',
	F = 'F',
	G = 'G',
	H = 'H',
	I = 'I',
	J = 'J',
	K = 'K',
	L = 'L',
	M = 'M',
	N = 'M',
	O = 'O',
	P = 'P',
	Q = 'Q',
	R = 'R',
	S = 'S',
	T = 'T',
	U = 'U',
	V = 'V',
	W = 'W',
	X = 'X',
	Y = 'Y',
	Z = 'Z',

	// numbers
	ONE = '1',
	TWO = '2',
	THREE = '3',
	FOUR = '4',
	FIVE = '5',
	SIX = '6',
	SEVEN = '7',
	EIGHT = '8',
	NINE = '9',
	ZERO = '0',

	// special characters (use arbritrary ids)
	SPACE = 0,
	L_ALT,
	R_ALT,
	L_CTRL,
	R_CTRL,
	L_SHIFT,
	R_SHIFT,
	CAPS,
	TAB,
	QUOTE,
	ESC,
	ENTER,
	BACKSPACE,
	MINUS,
	PLUS,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,

	NONE = std::numeric_limits<u32>::max()
};

enum class eMouseButton : u32
{
	LEFT,
	MIDDLE,
	RIGHT,
	SIDE1,
	SIDE2
};

}
