#pragma once

#include "Asciir/Core/Timing.h"
#include "Asciir/Core/Terminal.h"

namespace Asciir
{
	class EventListener
	{
	protected:
		const Terminal::EventCallbackFp& m_callback;

		// structure that holds key data and times key down events
		struct KeyInputData
		{
			bool is_down = false;
			bool is_toggled = false;

			duration time_since_down = duration();
		};

		// size of key input array for the keyboard
		static constexpr size_t KIS_LEN = 256;
		// size of key input array for the mouse
		static constexpr size_t MIS_LEN = 5;

		// updated for every event
		std::array<KeyInputData, KIS_LEN> keybd_state;
		// copies and store keybd_state every pollUpdate
		std::array<KeyInputData, KIS_LEN> keybd_poll_state;

		// same for mouse
		std::array<KeyInputData, MIS_LEN> mouse_state;
		std::array<KeyInputData, MIS_LEN> mouse_poll_state;

	public:

		EventListener() = default;
		virtual ~EventListener() = 0;

		virtual void start(const Terminal::EventCallbackFp& callback) = 0;
		virtual void stop() = 0;

		void pollState()
		{
			std::copy(keybd_state.begin(), keybd_state.end(), keybd_poll_state);
			std::copy(mouse_state.begin(), mouse_state.end(), mouse_poll_state);
		};

		const std::array<KeyInputData, KIS_LEN>& getKeybdPoll() { return keybd_poll_state; }
		const std::array<KeyInputData, MIS_LEN>& getMousePoll() { return mouse_poll_state; }
		
	};
}
