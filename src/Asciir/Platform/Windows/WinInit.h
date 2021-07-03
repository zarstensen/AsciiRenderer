#pragma once

#include "Asciir/Logging/Log.h"

namespace Asciir
{
	void WindowsInit()
	{
		// set the current process to high priority
		if (GetPriorityClass(GetCurrentProcess()) != HIGH_PRIORITY_CLASS)
		{
			AR_WIN_VERIFY(SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS));

			if (GetThreadPriority(GetCurrentThread()) != THREAD_PRIORITY_HIGHEST)
			{
				AR_WIN_VERIFY(SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST));
			}
		}
	}
}
