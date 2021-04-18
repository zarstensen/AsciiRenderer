#pragma once

namespace Asciir
{
	#ifdef AR_WIN

	template<typename T>
	auto win_assert(const T& arg);

	#define WIN_ASSERT win_assert

	#ifdef AR_DEBUG
	
	template<typename T>
	auto win_assert(const T& arg)
	{
		auto val = arg;
		if (!val)
		{
			TCHAR err_msg[256];
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), err_msg, 255, NULL);
			AR_ERR(err_msg);
		}
		return val;
	}

	#else

	template<typename T>
	inline auto win_assert(const T& arg) { return arg; }

	#endif
	#endif
}