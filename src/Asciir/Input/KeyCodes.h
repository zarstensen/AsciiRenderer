#pragma once

namespace Asciir
{
	enum class Key
	{
		UNKNOWN,
		SPACE,
		APOSTROPHE,
		COMMA,
		MINUS,
		PERIOD,
		SLASH,
		_0,
		_1,
		_2,
		_3,
		_4,
		_5,
		_6,
		_7,
		_8,
		_9,
		SEMICOLON,
		PLUS,
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		LEFT_BRACKET,
		RIGHT_BRACKET,
		BACKSLASH,
		GRAVE_ACCENT,
		WORLD_1,
		WORLD_2,
		ESCAPE,
		ENTER,
		TAB,
		BACKSPACE,
		INSERT,
		DEL,
		RIGHT,
		LEFT,
		DOWN,
		UP,
		PAGE_UP,
		PAGE_DOWN,
		HOME,
		END,
		CAPS_LOCK,
		SCROLL_LOCK,
		NUM_LOCK,
		PRINT_SCREEN,
		PAUSE,
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
		F13,
		F14,
		F15,
		F16,
		F17,
		F18,
		F19,
		F20,
		F21,
		F22,
		F23,
		F24,
		KP_0,
		KP_1,
		KP_2,
		KP_3,
		KP_4,
		KP_5,
		KP_6,
		KP_7,
		KP_8,
		KP_9,
		KP_DECIMAL,
		KP_DIVIDE,
		KP_MULTIPLY,
		KP_SUBTRACT,
		KP_ADD,
		KP_ENTER,
		KP_EQUAL,
		LEFT_SHIFT,
		LEFT_CONTROL,
		LEFT_ALT,
		LEFT_SUPER,
		RIGHT_SHIFT,
		RIGHT_CONTROL,
		RIGHT_ALT,
		RIGHT_SUPER,
		MENU,
		VOLUME_MUTE,
		VOLUME_DOWN,
		VOLUME_UP,
		STOP,
		SKIP_BACKWARD,
		PLAY_PAUSE,
		SKIP_FORWARD
	};

	constexpr int KEY_CODE_COUNT = 119;

	enum class MouseKey
	{
		UNKNOWN,
		LEFT_BUTTON,
		RIGHT_BUTTON,
		MIDDLE_BUTTON,
		BUTTON_1,
		BUTTON_2
	};

	constexpr int MOUSE_CODE_COUNT = 6;
}
