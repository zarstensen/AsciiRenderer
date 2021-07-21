#pragma once

#include "Asciir/Core/Timing.h"
#include "Asciir/Event/Event.h"
#include "Asciir/Input/KeyCodes.h"

#include "Asciir/Math/Vertices.h"

namespace Asciir
{
	class EventListener
	{
	public:
		using EventCallbackFp = std::function<void(Event&)>;

		// structures that holds key data and times key down events
		struct KeyInputData
		{
			bool is_down = false;
			bool is_repeat = false;

			duration time_since_down = duration();
		};

		struct MouseInputData
		{
			bool is_down = false;
			bool is_double_click = false;

			duration time_since_down = duration();
		};

		// size of key input array for the keyboard
		static constexpr size_t KIS_LEN = KEY_CODE_COUNT;
		// size of key input array for the mouse
		static constexpr size_t MIS_LEN = MOUSE_CODE_COUNT;

	protected:
		EventCallbackFp m_callback;
		// updated for every event
		std::array<KeyInputData, KIS_LEN> keybd_state;
		// only stores key down events. Gets cleared every call to pollState
		std::array<KeyInputData, KIS_LEN> keybd_down_state;
		// copies and store keybd_down_state every pollUpdate
		std::array<KeyInputData, KIS_LEN> keybd_poll_state;

		// same for mouse
		std::array<MouseInputData, MIS_LEN> mouse_state;
		std::array<MouseInputData, MIS_LEN> mouse_down_state;
		std::array<MouseInputData, MIS_LEN> mouse_poll_state;

		Coord m_mouse_pos;
		Coord m_poll_mouse_pos;
		TermVert m_poll_cur_pos;
		TermVert m_cur_pos;

	public:

		EventListener() {};
		virtual ~EventListener() {};

		virtual void start(EventCallbackFp callback) = 0;
		virtual void stop() = 0;

		void pollState()
		{
			keybd_poll_state = keybd_down_state;
			mouse_poll_state = mouse_down_state;
			keybd_down_state = keybd_state;
			mouse_down_state = mouse_state;
		};

		const std::array<KeyInputData, KIS_LEN>& getKeybdPoll() { return keybd_poll_state; }
		const std::array<MouseInputData, MIS_LEN>& getMousePoll() { return mouse_poll_state; }

		Coord getMousePosPoll() { return m_poll_mouse_pos; };
		Coord getMousePos() { return m_mouse_pos; };

		TermVert getCursorPosPoll() { return m_poll_cur_pos; };
		TermVert getCursorPos() { return m_cur_pos; };

		static Coord getGlobalMousePos();
	};
}
