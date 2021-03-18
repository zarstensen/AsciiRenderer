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
	static constexpr ATTRI TOP = 7;
	static constexpr ATTRI OVERLINED = 7;
	static constexpr ATTRI BOTTOM = 2;
	#ifdef AR_WIN
	static constexpr ATTRI LEFT = 5;
	static constexpr ATTRI RIGHT = 6;
	#else
	static constexpr ATTRI FRAMED = 5;
	static constexpr ATTRI ENCIRCLED = 6;
	#endif

	struct Color
	{
		unsigned char red, green, blue;

		Color();
		Color(unsigned char r, unsigned char g, unsigned char b);
		Color(unsigned char gray);
		Color(const Color& other);
	};
	
	typedef Color RGB24;

	struct RGB8
	{
		unsigned char red, green, blue;

		RGB8(unsigned char r, unsigned char g, unsigned char b);
		RGB8();

		Color getColor();
		operator Color();
	};
	
	struct RGB4
	{
		bool red, green, blue, intensity;

		RGB4();
		RGB4(bool r, bool g, bool b, bool i = false);

		Color getColor();
		operator Color();

	};

	struct GRAY8
	{
		unsigned char gray;

		GRAY8(unsigned char g);
		GRAY8();

		Color getColor();
		operator Color();
	};

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
