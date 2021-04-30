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
}
