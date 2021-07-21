#pragma once

#include "Asciir/Core/Core.h"
#include "Asciir/Math/Vertices.h"
#include "Asciir/Core/Terminal.h"

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

		static bool isFocused();

		static std::variant<std::monostate, KeyPressedEvent, KeyReleasedEvent> getKeyEvent(Key keycode);

		static std::variant<std::monostate, MousePressedEvent, MouseReleasedEvent> getMouseKeyEvent(MouseKey keycode);

		static MouseMovedEvent getMouseMovedEvent();

		static TerminalMovedEvent getTerminalMovedEvent();
		static TerminalResizedEvent getTerminalResizedEvent();

	protected:
		static void pollState(TerminalRenderer::TRUpdateInfo info);
		static Coord getMousePos();

		static void setEventListener(Ref<EventListener> listener);

		static Coord s_mouse_pos, s_mouse_diff;
		static Coord s_last_terminal_pos;
		static TermVert s_last_size;
		static TermVert s_cur_pos, s_cur_diff;
		static Terminal* i_terminal;
		static TerminalRenderer::TRUpdateInfo s_info;
		static Ref<EventListener> s_event_listener;
	};
}
