#pragma once

#include "Asciir/Core/Core.h"
#include "Asciir/Maths/Vertices.h"

#include "Asciir/Event/TerminalEventHandler.h"
#include "Asciir/Event/KeyEvent.h"
#include "Asciir/Event/MouseEvent.h"
#include "Asciir/Event/TerminalEvent.h"

#include "KeyCodes.h"

/// @brief how long a key should be held down before it is detected as pressed
#ifndef AR_KEY_PRESSED_TIMEOUT
#define AR_KEY_PRESSED_TIMEOUT 500
#endif

namespace Asciir
{
	/// @brief static interface that allows to read the latest polled input data from the keybaord, mouse and terminal.  
	///
	/// all functions, which need to access data about a specific key, either takes Asciir::Key or Asciir::MouseKey as an argument.
	/// 
	class Input
	{
		friend TerminalEventHandler;

	public:

		/// @brief returns wether the key is down.  
		/// 
		/// if the key is currently up, but it was pressed at least once inbetween the last poll and the current poll, this will return true.  
		/// if isKeyPressed() returns true, this will also return true.  
		/// 
		/// a input poll will be made before every application update.
		/// 
		/// @param keycode the key to check
		static bool isKeyDown(Key keycode);
		/// @brief returns wether the key is up.  
		/// @param keycode the key to check
		/// @note see implementation of isKeyDown() for specifics of when a key will be reported as up / down.
		static bool isKeyUp(Key keycode);

		/// @brief returns wether the key is pressed.  
		/// 
		/// if the key has continuously been held down for more than AR_KEY_PRESSED_TIMEOUT (default is 0.5 s), this will return true.
		/// 
		/// @param keycode the key to check
		static bool isKeyPressed(Key keycode);

		/// @brief return wether the key was toggled this poll.
		/// 
		/// if the key is down this poll, but was not down last poll, return true here.
		/// 
		/// @param keycode the key to check
		static bool isKeyToggled(Key keycode);
		/// @brief return wether the key was untoggled this poll.
		/// 
		/// if the key is up this poll, but was down last poll, return true here.
		/// 
		/// @param keycode the key code to check.
		/// @return 
		static bool isKeyUntoggled(Key keycode);

		/// @brief returns wether the mouse key is down.
		/// 
		/// for implementation details see isKeyDown()
		/// 
		/// @param keycode the mouse key to check.
		static bool isMouseDown(MouseKey keycode);
		/// @brief returns wether the mouse key is up.
		/// 
		/// for implementation details see isKeyUp()
		/// 
		/// @param keycode the mouse key to check.
		static bool isMouseUp(MouseKey keycode);

		/// @brief returns wether the mouse key was toggled this poll.
		/// 
		/// for implementation details see isKeyToggled()
		/// 
		/// @param keycode the mouse key to check.
		static bool isMouseToggled(MouseKey keycode);
		/// @brief returns wether the mouse key was untoggled this poll.
		/// 
		/// for implementation details see isKeyUntoggled()
		/// 
		/// @param keycode the mouse key to check
		static bool isMouseUntoggled(MouseKey keycode);

		/// @brief returns wether the mouse has moved since the last poll.
		static bool isMouseMoved();

		/// @brief returns wether the terminal has moved since the last poll.
		static bool isTerminalMoved();
		/// @brief returns wether the terminal has been resized since the last poll.
		static bool isTerminalResized();

		/// @brief returns wether the terminal is currently focused
		static bool isFocused();

		/// @brief gets the KeyEvent, if avaliable, for the passed key this poll 
		/// @param keycode key to get event from
		static std::variant<std::monostate, KeyPressedEvent, KeyReleasedEvent> getKeyEvent(Key keycode);

		/// @brief gets the MouseKeyEvent, if avaliable, for the passed mouse key this poll 
		/// @param keycode key to get event from
		static std::variant<std::monostate, MousePressedEvent, MouseReleasedEvent> getMouseKeyEvent(MouseKey keycode);
		
		/// @brief gets the MosueMovedEvent, for this poll 
		/// @param keycode key to get event from
		static MouseMovedEvent getMouseMovedEvent();

		/// @brief gets the TerminalMovedEvent for this poll
		static TerminalMovedEvent getTerminalMovedEvent();
		/// @brief gets the TerminalResizedEvent for this poll
		static TerminalResizedEvent getTerminalResizedEvent();

	protected:

		/// @brief polls the input state of the keyboard, mouse and terminal.
		/// @param info terminal info returned from the ARApp::render() function
		static void pollState(TerminalRenderer::TRUpdateInfo info);

		/// @brief gets the current mouse position
		static Coord getMousePos();

		/// @brief sets the eventlistener to be used for polling events
		static void setEventListener(EventListener& listener);

		static TermVert s_mouse_pos, s_mouse_diff;
		static TermVert s_last_terminal_pos;
		static TermVert s_last_size;
		static TermVert s_cur_pos, s_cur_diff;
		static TerminalEventHandler* i_terminal;
		static TerminalRenderer::TRUpdateInfo s_info;
		static EventListener* s_event_listener;
	};
}
