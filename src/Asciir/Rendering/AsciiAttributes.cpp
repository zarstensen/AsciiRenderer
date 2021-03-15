#include "AsciiAttributes.h"
#include <iostream>

namespace Asciir
{

	Color::Color()
		: red(0), green(0), blue(0)
	{}

	Color::Color(unsigned char r, unsigned char g, unsigned char b)
		: red(r), green(g), blue(b)
	{}

	Color::Color(const Color& other)
		: red(other.red), green(other.green), blue(other.blue)
	{}


	unsigned short Color::getColor() const
	{
		return 2;
	}
	Color::operator unsigned short() const
	{
		return getColor();
	}


	#ifdef AR_WIN
	AsciiAttr::AsciiAttr()
		: m_hConsole(GetStdHandle(STD_OUTPUT_HANDLE))
	{
		DWORD dwMode = 0;
		GetConsoleMode(m_hConsole, &dwMode);
		SetConsoleMode(m_hConsole, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
		

		
	}
	#endif
	AsciiAttr::~AsciiAttr()
	{
		
	}


	void AsciiAttr::setForeground(const Color& color)
	{
		m_foreground = color;
	}

	void AsciiAttr::setBackground(const Color& color)
	{
		m_background = color;
	}

	void AsciiAttr::setColor(const Color& foreground, const Color& background)
	{
		m_foreground = foreground;
		m_background = background;
	}


	void AsciiAttr::setBoxed(bool val)
	{
		attributes[TOP]	 = val;
		attributes[BOTTOM] = val;
		attributes[LEFT]	 = val;
		attributes[RIGHT]  = val;
	}

	void AsciiAttr::setLR(bool val)
	{
		attributes[LEFT] = val;
		attributes[RIGHT] = val;
	}

	void AsciiAttr::setTB(bool val)
	{
		attributes[TOP] = val;
		attributes[BOTTOM] = val;
	}


	void AsciiAttr::clear()
	{
		clearFormat();
		clearColor();
	}
	void AsciiAttr::clearFormat()
	{
		memset(attributes.data(), false, ATTR_COUNT);
		SetConsoleTextAttribute(m_hConsole, NULL);
	}
	void AsciiAttr::clearColor()
	{
		m_foreground = Color(255, 255, 255);
		m_background = Color(0, 0, 0);
	}


	void AsciiAttr::setAttribute(const ATTRI& attribute, bool val)
	{
		attributes[attribute] = val;
	}


	std::string AsciiAttr::ansiCode() const
	{
		size_t size = 3 + (5 + 4 * 3 )* 2;

		for (size_t i = 0; i < 5; i++)
		{
			size += 2 * attributes[i];
		}

		#ifndef AR_WIN
		for (size_t i = 5; i < 8; i++)
		{
			size += 3 * attributes[i];
		}
		#endif

		std::string escseq;
		escseq.reserve(size);

		ansiCode(escseq);

		return escseq;
	}

	void AsciiAttr::ansiCode(std::string& dst) const
	{
		dst += AR_ANSIS_CSI;

		if (attributes[BOLD])
			dst += "1;";

		if (attributes[ITALIC])
			dst += "3;";

		if (attributes[UNDERLINE])
			dst += "4;";

		if (attributes[BLINK])
			dst += "5;";

		if (attributes[STRIKE])
			dst += "3;";

		//foreground color
		dst += "38;2;";
		dst += std::to_string(m_foreground.red);
		dst += ";";
		dst += std::to_string(m_foreground.green);
		dst += ";";
		dst += std::to_string(m_foreground.blue);
		dst += ";";

		//background color
		dst += "48;2;";
		dst += std::to_string(m_background.red);
		dst += ";";
		dst += std::to_string(m_background.green);
		dst += ";";
		dst += std::to_string(m_background.blue);

		SetConsoleTextAttribute(m_hConsole,
			COMMON_LVB_GRID_HORIZONTAL * attributes[TOP]
			| COMMON_LVB_GRID_LVERTICAL * attributes[LEFT]
			| COMMON_LVB_GRID_RVERTICAL * attributes[RIGHT]);

		dst += 'm';
	}

	void AsciiAttr::ansiCode(std::ostream& stream) const
	{
		stream << AR_ANSIS_CSI;

		if (attributes[BOLD])
			stream << "1;";

		if (attributes[ITALIC])
			stream << "3;";

		if (attributes[UNDERLINE])
			stream << "4;";

		if (attributes[BLINK])
			stream << "5;";

		if (attributes[STRIKE])
			stream << "3;";

		//foreground color
		stream << "38;2;";
		stream << std::to_string(m_foreground.red);
		stream << ";";
		stream << std::to_string(m_foreground.green);
		stream << ";";
		stream << std::to_string(m_foreground.blue);
		stream << ";";

		//background color
		stream << "48;2;";
		stream << std::to_string(m_background.red);
		stream << ";";
		stream << std::to_string(m_background.green);
		stream << ";";
		stream << std::to_string(m_background.blue);

		SetConsoleTextAttribute(m_hConsole,
			COMMON_LVB_GRID_HORIZONTAL * attributes[TOP]
			| COMMON_LVB_GRID_LVERTICAL * attributes[LEFT]
			| COMMON_LVB_GRID_RVERTICAL * attributes[RIGHT]);

		stream << 'm';
	}


	void AsciiAttr::setTitle(const std::string& name)
	{
		std::cout << AR_ANSIS_OSC << "0;" << name << AR_ANSIS_CSI;
	}


	std::pair<short, short> AsciiAttr::terminalSize() const
	{
		CONSOLE_SCREEN_BUFFER_INFO console_info;
		GetConsoleScreenBufferInfo(m_hConsole, &console_info);
		short width  = console_info.srWindow.Right  - console_info.srWindow.Left + 1;
		short height = console_info.srWindow.Bottom - console_info.srWindow.Top  + 1;

		return { width, height };
	}

	
	std::ostream& Asciir::operator<<(std::ostream& stream, const AsciiAttr& other)
	{
		other.ansiCode(stream);
		return stream;
	}
}
