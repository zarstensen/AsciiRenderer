#pragma once

#include "AsciiAttributes.h"

namespace Asciir
{
	constexpr auto AR_ANSIS_CSI = "\x1b[";
	constexpr auto AR_ANSIS_OSC = "\x1b]";

	/// @breif windows does not support bold characters. this is accounted for by changing the brightness of the Colour by AR_BOLD_DIFF
	constexpr auto AR_BOLD_DIFF = 38;

	constexpr unsigned char IS_BLACK = 0b0000;
	constexpr unsigned char IS_RED = 0b0001;
	constexpr unsigned char IS_GREEN = 0b0010;
	constexpr unsigned char IS_BLUE = 0b0100;
	constexpr unsigned char IS_YELLOW = 0b0011;
	constexpr unsigned char IS_MAGENTA = 0b0101;
	constexpr unsigned char IS_CYAN = 0b0110;
	constexpr unsigned char IS_WHITE = 0b1111;
	constexpr unsigned char IS_IBLACK = 0b1000;
	constexpr unsigned char IS_IRED = 0b1001;
	constexpr unsigned char IS_IGREEN = 0b1010;
	constexpr unsigned char IS_IBLUE = 0b1100;
	constexpr unsigned char IS_IYELLOW = 0b1011;
	constexpr unsigned char IS_IMAGENTA = 0b1101;
	constexpr unsigned char IS_ICYAN = 0b1110;
	constexpr unsigned char IS_IWHITE = 0b1111;

	/// @brief colour gradient for 16 bit colour mode
	constexpr unsigned char CGRADIENT16[] = { 0, 95, 135, 175, 215, 255 };
	
#ifdef AR_WIN

	static constexpr WORD DEFAULT_FOREGROUND = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

	// I = Intense

	static const auto BLACK8 = Colour(12, 12, 12);
	static const auto RED8 = Colour(197, 15, 31);
	static const auto GREEN8 = Colour(19, 161, 14);
	static const auto BLUE8 = Colour(0, 55, 218);
	static const auto YELLOW8 = Colour(193, 156, 0);
	static const auto MAGENTA8 = Colour(136, 23, 152);
	static const auto CYAN8 = Colour(58, 150, 221);
	static const auto WHITE8 = Colour(204, 204, 204);
	static const auto IBLACK8 = Colour(118, 118, 118);
	static const auto GRAY8 = IBLACK8;
	static const auto GREY8 = GRAY8;
	static const auto IRED8 = Colour(231, 72, 86);
	static const auto IGREEN8 = Colour(22, 198, 12);
	static const auto IBLUE8 = Colour(59, 120, 255);
	static const auto IYELLOW8 = Colour(249, 241, 165);
	static const auto IMAGENTA8 = Colour(180, 0, 158);
	static const auto ICYAN8 = Colour(97, 214, 214);
	static const auto IWHITE8 = Colour(242, 242, 242);

#else
	static const auto BLACK8 = Colour(12, 12, 12);
	static const auto RED8 = Colour(197, 15, 31);
	static const auto GREEN8 = Colour(19, 161, 14);
	static const auto BLUE8 = Colour(0, 55, 218);
	static const auto YELLOW8 = Colour(193, 156, 0);
	static const auto MAGENTA8 = Colour(136, 23, 152);
	static const auto CYAN8 = Colour(58, 150, 221);
	static const auto WHITE8 = Colour(204, 204, 204);
	static const auto IBLACK8 = Colour(118, 118, 118);
	static const auto GRAY8 = IBLACK8;
	static const auto GREY8 = GRAY8;
	static const auto IRED8 = Colour(231, 72, 86);
	static const auto IGREEN8 = Colour(22, 198, 12);
	static const auto IBLUE8 = Colour(59, 120, 255);
	static const auto IYELLOW8 = Colour(249, 241, 165);
	static const auto IMAGENTA8 = Colour(180, 0, 158);
	static const auto ICYAN8 = Colour(97, 214, 214);
	static const auto IWHITE8 = Colour(242, 242, 242);

#endif
}