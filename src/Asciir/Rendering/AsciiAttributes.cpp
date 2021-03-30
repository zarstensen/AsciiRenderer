#include "arpch.h"
#include "AsciiAttributes.h"
#include "RenderConsts.h"

namespace Asciir
{
	Color::Color()
		: red(0), green(0), blue(0)
	{}

	Color::Color(unsigned char r, unsigned char g, unsigned char b)
		: red(r), green(g), blue(b)
	{}

	Color::Color(unsigned char gray)
		: red(gray), green(gray), blue(gray)
	{}

	Color::Color(const Color& other)
		: red(other.red), green(other.green), blue(other.blue)
	{}

	bool Color::operator==(const Color& other)
	{
		return red == other.red && green == other.green && blue == other.blue;
	}
	bool Color::operator!=(const Color& other)
	{
		return !operator==(other);
	}
	bool Color::operator<(const Color& other)
	{
		return (red + green + blue) / 3 < (other.red + other.green + other.blue) / 3;
	}
	bool Color::operator>(const Color& other)
	{
		return (red + green + blue) / 3 > (other.red + other.green + other.blue) / 3;
	}
	bool Color::operator<=(const Color& other)
	{
		return (red + green + blue) / 3 <= (other.red + other.green + other.blue) / 3;
	}
	bool Color::operator>=(const Color& other)
	{
		return (red + green + blue) / 3 >= (other.red + other.green + other.blue) / 3;
	}


	RGB8::RGB8(unsigned char r, unsigned char g, unsigned char b)
	{
		red = r;
		green = g;
		blue = b;

		#ifdef AR_DEBUG
		if (red < 6)
		{
			throw std::runtime_error("Red color value must be less than 6, not '" + std::to_string(red) + "'");
		}
		if (green < 6)
		{
			throw std::runtime_error("Green color value must be less than 6, not '" + std::to_string(green) + "'");
		}
		if (blue < 6)
		{
			throw std::runtime_error("Blue color value must be less than 6, not '" + std::to_string(blue) + "'");
		}
		#endif
	}

	RGB8::RGB8()
		: red(0), green(0), blue(0)
	{}

	Color RGB8::getColor()
	{
		#ifdef AR_DEBUG
		if (red > 5)
		{
			throw std::runtime_error("Red color value must be less than 6, not '" + std::to_string(red) + "'");
		}
		if (green > 5)
		{
			throw std::runtime_error("Green color value must be less than 6, not '" + std::to_string(green) + "'");
		}
		if (blue > 5)
		{
			throw std::runtime_error("Blue color value must be less than 6, not '" + std::to_string(blue) + "'");
		}
		#endif



		return Color(CGRADIENT16[red], CGRADIENT16[green], CGRADIENT16[blue]);
	}

	RGB8::operator Color()
	{
		return getColor();
	}

	GRAY8::GRAY8(unsigned char g)
		: gray(g)
	{
		#ifdef AR_DEBUG
		if (gray < 24)
		{
			throw std::runtime_error("Gray value must be less than 24, not '" + std::to_string(gray) + "'");
		}
		#endif
	}

	GRAY8::GRAY8()
		: gray(0)
	{}

	Color GRAY8::getColor()
	{
		#ifdef AR_DEBUG
		if (gray < 24)
		{
			throw std::runtime_error("Gray value must be less than 24, not '" + std::to_string(gray) + "'");
		}
		#endif
		
		return 8 + gray * 10;
	}

	GRAY8::operator Color()
	{
		return getColor();
	}

	RGB4::RGB4()
		: red(0), green(0), blue(0), intensity(0)
	{}

	RGB4::RGB4(bool r, bool g, bool b, bool i)
		: red(r), green(g), blue(b), intensity(i)
	{}

	// determine the RGB values of 4 bit color system. The colors are platform dependent
	Color RGB4::getColor()
	{
		switch (BIT_SHL(0, red) | BIT_SHL(1, green) | BIT_SHL(2, blue) | BIT_SHL(intensity, 3))
		{
			case IS_BLACK:
				return BLACK8;
				break;
			case IS_RED:
				return RED8;
				break;
			case IS_GREEN:
				return GREEN8;
				break;
			case IS_BLUE:
				return BLUE8;
				break;
			case IS_YELLOW:
				return YELLOW8;
				break;
			case IS_MAGENTA:
				return MAGENTA8;
				break;
			case IS_CYAN:
				return CYAN8;
				break;
			case IS_IBLACK:
				return IBLACK8;
				break;
			case IS_IRED:
				return IRED8;
				break;
			case IS_IGREEN:
				return IGREEN8;
				break;
			case IS_IBLUE:
				return IBLUE8;
				break;
			case IS_IYELLOW:
				return IYELLOW8;
				break;
			case IS_IMAGENTA:
				return IMAGENTA8;
				break;
			case IS_ICYAN:
				return ICYAN8;
				break;
			default:
				assert("Invalid color");
				return NULL;
		}
		
	}

	RGB4::operator Color()
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
		clearColor();
	}
	#endif
	AsciiAttr::~AsciiAttr()
	{
	}

	void AsciiAttr::setForeground(const Color& color)
	{
		m_foreground = color;
	}

	Color AsciiAttr::getForeground()
	{
		return m_foreground;
	}

	void AsciiAttr::setBackground(const Color& color)
	{
		m_background = color;
	}

	Color AsciiAttr::getBackground()
	{
		return m_background;
	}

	void AsciiAttr::setColor(const Color& foreground, const Color& background)
	{
		m_foreground = foreground;
		m_background = background;
	}

	void AsciiAttr::setBoxed(bool val)
	{
		attributes[TOP] = val;
		attributes[BOTTOM] = val;
		attributes[LEFT] = val;
		attributes[RIGHT] = val;
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
		SetConsoleTextAttribute(m_hConsole, DEFAULT_FOREGROUND);
	}
	void AsciiAttr::clearColor()
	{
		setForeground(Color(204, 204, 204));

		setBackground(Color(10, 10, 10));
	}

	void AsciiAttr::setAttribute(const ATTRI& attribute, bool val)
	{
		attributes[attribute] = val;
	}

	std::string AsciiAttr::ansiCode() const
	{
		size_t size = 3 + (5 + 4 * 3) * 2;

		#ifdef AR_WIN
		for (size_t i = 1; i < 5; i++)
		{
			size += 2 * attributes[i];
		}

		#else
		for (size_t i = 0; i < 5; i++)
		{
			size += 2 * attributes[i];
		}

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
		// disabled because it causes a large overhead
		#if 0
		SetConsoleTextAttribute(m_hConsole, DEFAULT_FOREGROUND
			| COMMON_LVB_GRID_HORIZONTAL * attributes[TOP]
			| COMMON_LVB_GRID_LVERTICAL * attributes[LEFT]
			| COMMON_LVB_GRID_RVERTICAL * attributes[RIGHT]);
		#endif

		// formatting
		dst += AR_ANSIS_CSI;
		dst += "0";

		#ifndef AR_WIN
		if (attributes[BOLD])
			dst += ";1";
		#endif

		if (attributes[ITALIC])
			dst += ";3";

		if (attributes[UNDERLINE])
			dst += ";4";

		if (attributes[BLINK])
			dst += ";5";

		if (attributes[STRIKE])
			dst += ";9";

		// foreground color

		#ifdef AR_WIN
		if (attributes[BOLD])
		{
			unsigned char red = m_foreground.red + AR_BOLD_DIFF;
			unsigned char green = m_foreground.green + AR_BOLD_DIFF;
			unsigned char blue = m_foreground.blue + AR_BOLD_DIFF;

			red = red > m_foreground.red ? red : 255;
			green = green > m_foreground.green ? green : 255;
			blue = blue > m_foreground.blue ? blue : 255;

			dst += ";38;2;";
			dst += std::to_string(red);
			dst += ";";
			dst += std::to_string(green);
			dst += ";";
			dst += std::to_string(blue);
			dst += ";";
		}
		else
		{
			dst += ";38;2;";
			dst += std::to_string(m_foreground.red);
			dst += ";";
			dst += std::to_string(m_foreground.green);
			dst += ";";
			dst += std::to_string(m_foreground.blue);
			dst += ";";
		}
		#else
		dst += ";38;2;";
		dst += std::to_string(m_foreground.red);
		dst += ";";
		dst += std::to_string(m_foreground.green);
		dst += ";";
		dst += std::to_string(m_foreground.blue);
		dst += ";";
		#endif

		// background color
		dst += "48;2;";
		dst += std::to_string(m_background.red);
		dst += ";";
		dst += std::to_string(m_background.green);
		dst += ";";
		dst += std::to_string(m_background.blue);
		dst += 'm';
	}

	void AsciiAttr::ansiCode(std::ostream& stream) const
	{
		
		#if 0
		SetConsoleTextAttribute(m_hConsole, DEFAULT_FOREGROUND
			| COMMON_LVB_GRID_HORIZONTAL * attributes[TOP]
			| COMMON_LVB_GRID_LVERTICAL * attributes[LEFT]
			| COMMON_LVB_GRID_RVERTICAL * attributes[RIGHT]);
		#endif

		// formatting
		stream << AR_ANSIS_CSI;
		stream << "0";

		#ifndef AR_WIN
		if (attributes[BOLD])
			stream << ";1";
		#endif

		if (attributes[ITALIC])
			stream << ";3";

		if (attributes[UNDERLINE])
			stream << ";4";

		if (attributes[BLINK])
			stream << ";5";

		if (attributes[STRIKE])
			stream << ";3";

		// foreground color
		#ifdef AR_WIN
		if (attributes[BOLD])
		{
			unsigned char red = m_foreground.red + AR_BOLD_DIFF;
			unsigned char green = m_foreground.green + AR_BOLD_DIFF;
			unsigned char blue = m_foreground.blue + AR_BOLD_DIFF;

			red = red > m_foreground.red ? red : 255;
			green = green > m_foreground.green ? green : 255;
			blue = blue > m_foreground.blue ? blue : 255;

			stream << ";38;2;";
			stream << std::to_string(red);
			stream << ";";
			stream << std::to_string(green);
			stream << ";";
			stream << std::to_string(blue);
			stream << ";";
		}
		else
		{
			stream << ";38;2;";
			stream << std::to_string(m_foreground.red);
			stream << ";";
			stream << std::to_string(m_foreground.green);
			stream << ";";
			stream << std::to_string(m_foreground.blue);
			stream << ";";
		}
		#else
		stream << ";38;2;";
		stream << std::to_string(m_foreground.red);
		stream << ";";
		stream << std::to_string(m_foreground.green);
		stream << ";";
		stream << std::to_string(m_foreground.blue);
		stream << ";";
		#endif

		// background color
		stream << "48;2;";
		stream << std::to_string(m_background.red);
		stream << ';';
		stream << std::to_string(m_background.green);
		stream << ';';
		stream << std::to_string(m_background.blue);
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
		short width = console_info.srWindow.Right - console_info.srWindow.Left + 1;
		short height = console_info.srWindow.Bottom - console_info.srWindow.Top + 1;

		return { width, height };
	}

	std::ostream& Asciir::operator<<(std::ostream& stream, const AsciiAttr& other)
	{
		other.ansiCode(stream);
		return stream;
	}
}