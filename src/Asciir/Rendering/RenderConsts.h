#pragma once

#include "AsciiAttributes.h"

namespace Asciir
{
	constexpr auto AR_ANSIS_CSI = "\x1b[";
	constexpr auto AR_ANSIS_OSC = "\x1b]";

	// windows does not support bold characters. this is accounted for by changing the brightness of the color by AR_BOLD_DIFF
	constexpr auto AR_BOLD_DIFF = 38;

	constexpr unsigned char IS_BLACK =		0b0000;
	constexpr unsigned char IS_RED =		0b0001;
	constexpr unsigned char IS_GREEN =		0b0010;
	constexpr unsigned char IS_BLUE =		0b0100;
	constexpr unsigned char IS_YELLOW =		0b0011;
	constexpr unsigned char IS_MAGENTA =	0b0101;
	constexpr unsigned char IS_CYAN =		0b0110;
	constexpr unsigned char IS_WHITE =		0b1111;
	constexpr unsigned char IS_IBLACK =		0b1000;
	constexpr unsigned char IS_IRED =		0b1001;
	constexpr unsigned char IS_IGREEN =		0b1010;
	constexpr unsigned char IS_IBLUE =		0b1100;
	constexpr unsigned char IS_IYELLOW =	0b1011;
	constexpr unsigned char IS_IMAGENTA =	0b1101;
	constexpr unsigned char IS_ICYAN =		0b1110;
	constexpr unsigned char IS_IWHITE =		0b1111;

	constexpr unsigned char CGRADIENT16[] = { 0, 95, 135, 175, 215, 255 };

	#ifdef AR_WIN

	static constexpr WORD DEFAULT_FOREGROUND = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

	static const auto BLACK8 = Color(12, 12, 12);
	static const auto RED8 = Color(197, 15, 31);
	static const auto GREEN8 = Color(19, 161, 14);
	static const auto BLUE8 = Color(0, 55, 218);
	static const auto YELLOW8 = Color(193, 156, 0);
	static const auto MAGENTA8 = Color(136, 23, 152);
	static const auto CYAN8 = Color(58, 150, 221);
	static const auto WHITE8 = Color(204, 204, 204);
	static const auto IBLACK8 = Color(118, 118, 118);
	static const auto GRAY8 = IBLACK8;
	static const auto GREY8 = GRAY8;
	static const auto IRED8 = Color(231, 72, 86);
	static const auto IGREEN8 = Color(22, 198, 12);
	static const auto IBLUE8 = Color(59, 120, 255);
	static const auto IYELLOW8 = Color(249, 241, 165);
	static const auto IMAGENTA8 = Color(180, 0, 158);
	static const auto ICYAN8 = Color(97, 214, 214);
	static const auto IWHITE8 = Color(242, 242, 242);

	#else
	static const auto BLACK8 = Color(12, 12, 12);
	static const auto RED8 = Color(197, 15, 31);
	static const auto GREEN8 = Color(19, 161, 14);
	static const auto BLUE8 = Color(0, 55, 218);
	static const auto YELLOW8 = Color(193, 156, 0);
	static const auto MAGENTA8 = Color(136, 23, 152);
	static const auto CYAN8 = Color(58, 150, 221);
	static const auto WHITE8 = Color(204, 204, 204);
	static const auto IBLACK8 = Color(118, 118, 118);
	static const auto GRAY8 = IBLACK8;
	static const auto GREY8 = GRAY8;
	static const auto IRED8 = Color(231, 72, 86);
	static const auto IGREEN8 = Color(22, 198, 12);
	static const auto IBLUE8 = Color(59, 120, 255);
	static const auto IYELLOW8 = Color(249, 241, 165);
	static const auto IMAGENTA8 = Color(180, 0, 158);
	static const auto ICYAN8 = Color(97, 214, 214);
	static const auto IWHITE8 = Color(242, 242, 242);

	#endif

	
}