#pragma once

#ifdef AR_WIN
#include <Windows.h>
#else
#include <sys/ioctl>
#include <unistd>
#endif

#include "../Core/Core.h"
#include <tuple>
#include <string>
#include <array>
#include <iostream>

namespace Asciir
{
	static constexpr size_t ATTR_COUNT = 8;

	typedef unsigned short ATTRI;

	static constexpr ATTRI BOLD = 0;
	static constexpr ATTRI ITALIC = 1;
	static constexpr ATTRI UNDERLINE = 2;
	static constexpr ATTRI BLINK = 3;
	static constexpr ATTRI STRIKE = 4;
	#ifdef AR_WIN
	static constexpr ATTRI LEFT = 5;
	static constexpr ATTRI RIGHT = 6;
	static constexpr ATTRI TOP = 7;
	static constexpr ATTRI BOTTOM = 2;
	#else
	static constexpr ATTRI FRAMED = 5;
	static constexpr ATTRI ENCIRCLED = 6;
	static constexpr ATTRI OVERLINED = 7;
	#endif

	struct Color
	{
		unsigned char red, green, blue;

		Color();
		Color(unsigned char r, unsigned char g, unsigned char b);
		Color(const Color& other);

		unsigned short getColor() const;

		operator unsigned short() const;
	};

	#ifdef AR_WIN
	static constexpr WORD DEFAULT_FOREGROUND = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	#endif
	/*static const AscColor BLACK(0, 0, 0, 0);
	static const AscColor BLUE(0, 0, 1, 1);
	static const AscColor CYAN(0, 1, 1, 1);
	static const AscColor DARK_BLUE(0, 0, 1, 0);
	static const AscColor DARK_CYAN(0, 1, 1, 0);
	static const AscColor DARK_GRAY(0, 0, 0, 1);
	static const AscColor DARK_GREEN(0, 1, 0, 0);
	static const AscColor PURPLE(1, 0, 1, 0);
	static const AscColor DARK_RED(1, 0, 0, 0);
	static const AscColor ORANGE(1, 1, 0, 0);
	static const AscColor GRAY(1, 1, 1, 0);
	static const AscColor GREEN(0, 1, 0, 1);
	static const AscColor MAGENTA(1, 0, 1, 1);
	static const AscColor RED(1, 0, 0, 1);
	static const AscColor WHITE(1, 1, 1, 1);
	static const AscColor YELLOW(1, 1, 0, 1);*/

	/*
	* windows does not support
	*
	* Blink
	* Strike
	* Framed
	* Encircled
	* Overlined = Top
	*
	* only windows supports
	*
	* Left
	* Right
	* Top = Overlined
	* Bottom
	*
	* All of the above = Framed
	*/

	class AsciiAttr
	{
		#ifdef AR_WIN
		HANDLE m_hConsole;
		#endif

		Color m_foreground;
		Color m_background;

	public:
		std::array<bool, ATTR_COUNT> attributes;

		AsciiAttr();
		~AsciiAttr();

		void setForeground(const Color& color);
		void setBackground(const Color& color);
		void setColor(const Color& foreground, const Color& background);

		void clear();
		void clearFormat();
		void clearColor();

		void setAttribute(const ATTRI& attribute, bool val);

		#ifdef AR_WIN

		void setBoxed(bool val);
		void setLR(bool val);
		void setTB(bool val);

		#endif

		std::string ansiCode() const;
		void ansiCode(std::string& dst) const;
		void ansiCode(std::ostream& stream) const;

		void setTitle(const std::string& name);

		std::pair<short, short> terminalSize() const;
	};

	std::ostream& operator<<(std::ostream& stream, const AsciiAttr& other);
}
