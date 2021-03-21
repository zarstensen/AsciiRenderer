#pragma once

#ifndef AR_WIN
#error Only windows is currently supported
#endif

namespace Asciir
{
	constexpr int BIT_SHL(int x, int a = 1)
	{
		return a << x;
	}
}
