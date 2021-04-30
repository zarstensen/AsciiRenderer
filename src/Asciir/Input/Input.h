#pragma once

#include "Asciir/Core/Core.h"
#include "Asciir/Rendering/Vertices.h"
#include "Asciir/Core/Terminal.h"

#ifdef AR_WIN
#include "Asciir/Platform/Windows/WindowsTerminal.h"
#endif

#include "Asciir/Event/KeyEvent.h"
#include "Asciir/Event/MouseEvent.h"
#include "Asciir/Event/TerminalEvent.h"

#include "KeyCodes.h"

#ifndef AR_KEY_PRESSED_TIMEOUT
#define AR_KEY_PRESSED_TIMEOUT 500
#endif

namespace Asciir
{
	class Input
	{

		friend Terminal;

		#ifdef AR_WIN
		friend WindowsTerminal;
		#endif

	protected:
		static Coord s_last_mouse_pos;
		static Coord s_last_terminal_pos;
		static TermVert s_last_size;
		static Terminal* i_terminal;
		static TRUpdateInfo s_info;

	public:

		static bool isKeyDown(Key keycode);
		static bool isKeyUp(Key keycode);

		static bool isKeyPressed(Key keycode);

		static bool isKeyToggled(Key keycode);
		static bool isKeyUntoggled(Key keycode);

		static bool isMouseDown(MouseKey keycode);
		static bool isMouseUp(MouseKey keycode);

		static bool isMouseToggled(MouseKey keycode);
		static bool isMouseUntoggled(MouseKey keycode);

		static bool isMouseMoved();

		static bool isTerminalMoved();
		static bool isTerminalResized();

		static std::variant<std::monostate, KeyPressedEvent, KeyReleasedEvent> getKeyEvent(Key keycode);

		static std::variant<std::monostate, MousePressedEvent, MouseReleasedEvent> getMouseKeyEvent(MouseKey keycode);

		static MouseMovedEvent getMouseMovedEvent();

		static TerminalMovedEvent getTerminalMovedEvent();
		static TerminalResizedEvent getTerminalResizedEvent();

	protected:
		static void update(TRUpdateInfo info);
	};

}