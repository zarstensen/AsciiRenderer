#pragma once

#include "Asciir/Input/Input.h"
#include "Asciir/Core/Timing.h"

namespace Asciir
{

	int charToKey(char c);

	// structure that holds key data and times key down events
	struct KeyInputData
	{
		bool is_down = false;
		bool is_toggled = false;
		
		duration time_since_down = duration();
	};

	// array will be updated with a call to GetKeyboardState function every engine update
	KeyInputData WIN_KEY_MAP[256];


	Coord getMousePos();

	
	// if AR_DETECT_KEY_ON_FRAME is not defined a key pressed will also be detected between engine updates.
	#if AR_DETECT_KEY_ON_FRAME
	static constexpr short async_key_check = 0x0001;
	#else
	static constexpr short async_key_check = 0x8000;
	#endif

}
