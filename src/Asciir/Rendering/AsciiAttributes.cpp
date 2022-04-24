#include "arpch.h"
#include "AsciiAttributes.h"
#include "TerminalRenderer.h"
#include "RenderConsts.h"
#include "Asciir/Logging/Log.h"

namespace Asciir
{
	Colour::Colour()
		: red(0), green(0), blue(0), alpha(UCHAR_MAX)
	{}

	Colour::Colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
		: red(r), green(g), blue(b), alpha(a)
	{}

	Colour::Colour(unsigned char grey, unsigned char a)
		: red(grey), green(grey), blue(grey), alpha(a)
	{}

	Colour::Colour(const Colour& other)
		: red(other.red), green(other.green), blue(other.blue), alpha(other.alpha)
	{}

	Colour Colour::inverse() const
	{
		return Colour(UCHAR_MAX - red, UCHAR_MAX - green, UCHAR_MAX - blue);
	}


	bool Colour::operator==(const Colour& other) const
	{
		return red == other.red && green == other.green && blue == other.blue;
	}

	bool Colour::operator!=(const Colour& other) const
	{
		return !operator==(other);
	}

	Colour& Colour::blend(const Colour& other)
	{
		Real foreground_alpha = (Real)other.alpha / UCHAR_MAX;

		red = (unsigned char)(foreground_alpha * other.red + (1 - foreground_alpha) * (Real)red);
		green = (unsigned char)(foreground_alpha * other.green + (1 - foreground_alpha) * (Real)green);
		blue = (unsigned char)(foreground_alpha * other.blue + (1 - foreground_alpha) * (Real)blue);
		alpha = (unsigned char)(alpha + (1 - (Real)alpha / UCHAR_MAX) * foreground_alpha);

		return *this;
	}

	Colour Colour::blend(const Colour& background, const Colour& colour)
	{
		Colour result = background;
		result.blend(colour);
		return result;
	}

	RGB8::RGB8(unsigned char r, unsigned char g, unsigned char b)
		: red(r), green(g), blue(b)
	{
		AR_ASSERT_MSG(red < 6, "Red colour value must be less than 6. value: ", red);
		AR_ASSERT_MSG(green < 6, "Green colour value must be less than 6. value: ", green);
		AR_ASSERT_MSG(blue < 6, "Blue colour value must be less than 6. value: ", blue);
	}

	// TODO: double check this actually works.
	RGB8::RGB8(const Colour& colour)
		: red((colour.red * 6) / 255), green((colour.green * 6) / 255), blue((colour.blue * 6) / 255)
	{}

	RGB8::RGB8()
		: red(0), green(0), blue(0)
	{}

	Colour RGB8::getColour()
	{
		return Colour(CGRADIENT16[red], CGRADIENT16[green], CGRADIENT16[blue]);
	}

	RGB8::operator Colour()
	{
		return getColour();
	}
	
	unsigned int RGB8::getIndx()
	{
		return blue + (green + red * 6) * 6;
	}

	GRAY8::GRAY8(unsigned char g)
		: grey(g)
	{
		AR_ASSERT_MSG(grey < 24, "Gray value must be less than 24. value: ", grey);
	}

	GRAY8::GRAY8()
		: grey(0)
	{}

	Colour GRAY8::getColour()
	{
		// converts grey colour from range (0-23) to an 8 bit value (8-238)
		return 8 + grey * 10;
	}

	GRAY8::operator Colour()
	{
		return getColour();
	}

	RGB4::RGB4()
		: red(0), green(0), blue(0), intensity(0)
	{}

	RGB4::RGB4(bool r, bool g, bool b, bool i)
		: red(r), green(g), blue(b), intensity(i)
	{}

	// determine the RGB values of 4 bit colour system. The colours are platform dependent
	Colour RGB4::getColour()
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
			AR_ASSERT_MSG(false, "Invalid Colour");
			return {};
		}
	}

	RGB4::operator Colour()
	{
		return getColour();
	}

	// constructor and destructor is platform dependent

	AsciiAttr::~AsciiAttr() {}

	void AsciiAttr::setForeground(const Colour& colour)
	{
		m_foreground = colour;
	}

	Colour AsciiAttr::getForeground()
	{
		return m_foreground;
	}

	void AsciiAttr::setBackground(const Colour& colour)
	{
		m_background = colour;
	}

	Colour AsciiAttr::getBackground()
	{
		return m_background;
	}

	void AsciiAttr::setColour(const Colour& foreground, const Colour& background)
	{
		m_foreground = foreground;
		m_background = background;
	}

#ifdef AR_WIN

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

#endif

	void AsciiAttr::clear()
	{
		clearFormat();
		clearColour();
		m_cleared = true;
	}

	void AsciiAttr::clearFormat()
	{
		memset(attributes.data(), false, ATTR_COUNT);
	}

	void AsciiAttr::clearColour()
	{
		setForeground(WHITE8);
		m_last_foreground = getForeground();

		setBackground(BLACK8);
		m_last_background = getBackground();
	}

	void AsciiAttr::setAttribute(const ATTRI& attribute, bool val)
	{
		attributes[attribute] = val;
	}

	void AsciiAttr::move(TermVert pos)
	{
		m_pos = pos;
		m_should_move = true;
	}

	void AsciiAttr::moveCode(std::string& dst)
	{
		if (m_should_move)
		{
			dst += AR_ANSI_CSI;
			dst += std::to_string(m_pos.y + 1) + ';' + std::to_string(m_pos.x + 1) + 'H';

			m_should_move = false;
		}
	}


	void AsciiAttr::moveCode(std::ostream& stream)
	{
		if (m_should_move)
		{
			stream << AR_ANSI_CSI;
			stream << std::to_string(m_pos.y + 1) << ';' << std::to_string(m_pos.x + 1) << 'H';

			m_should_move = false;
		}
	}

	// helper function for converting an integer to a string, with known maximum integer length, in order to avoid heap allocation
	template<size_t w, typename T>
	void fixedToString(T value, char* out)
	{
		snprintf(out, w + 1, "%d", value);
	}

	std::string AsciiAttr::ansiCode()
	{
		// calculate the size in order to minimize allocations

		// 2 bytes for the escape sequence start
		// 1 byte for the reset attribute
		// 5 bytes for the set colour code ';38/48;5'
		// 4 bytes for a single channel 'xxx;' multiply by 3, as there are 3 channels (rgb)
		// sets both foreground and background colour
		size_t size = 2 + 1 + (5 + 4 * 3) * 2;

		for (size_t i = 1; i < 5; i++)
		{
			// each attribute will end up taking 2 bytes of space ';i' 
			size += 2ULL * attributes[i];
		}

		std::string escseq;
		escseq.reserve(size);

		ansiCode(escseq);

		return escseq;
	}

	void AsciiAttr::ansiCode(std::string& dst)
	{
		// disabled because it causes a large overhead
	#if 0
		SetConsoleTextAttribute(m_hConsole, DEFAULT_FOREGROUND
			| COMMON_LVB_GRID_HORIZONTAL * attributes[TOP]
			| COMMON_LVB_GRID_LVERTICAL * attributes[LEFT]
			| COMMON_LVB_GRID_RVERTICAL * attributes[RIGHT]);
	#endif

		// cursor

		moveCode(dst);

		// formatting
		dst += AR_ANSI_CSI;
		dst += "0";

		if (attributes[ITALIC])
			dst += ";3";

		if (attributes[UNDERLINE])
			dst += ";4";

		if (attributes[BLINK])
			dst += ";5";

		if (attributes[STRIKE])
			dst += ";9";

		// colour string buffer
		char colour_buffer[4];

		// foreground colour

		unsigned char red = m_foreground.red;
		unsigned char green = m_foreground.green;
		unsigned char blue = m_foreground.blue;

		if (attributes[BOLD])
		{
			red += AR_BOLD_DIFF;
			green += AR_BOLD_DIFF;
			blue += AR_BOLD_DIFF;

			red = red > m_foreground.red ? red : 255;
			green = green > m_foreground.green ? green : 255;
			blue = blue > m_foreground.blue ? blue : 255;
		}

		dst += ";38;2;";
		fixedToString<3>(red, colour_buffer);
		dst += colour_buffer;
		dst += ";";
		fixedToString<3>(green, colour_buffer);
		dst += colour_buffer;
		dst += ";";
		fixedToString<3>(blue, colour_buffer);
		dst += colour_buffer;
		dst += ";";
		

		// background colour
		dst += "48;2;";
		fixedToString<3>(m_background.red, colour_buffer);
		dst += colour_buffer;
		dst += ";";
		fixedToString<3>(m_background.green, colour_buffer);
		dst += colour_buffer;
		dst += ";";
		fixedToString<3>(m_background.blue, colour_buffer);
		dst += colour_buffer;
		dst += ";";
	}

	void AsciiAttr::ansiCode(std::ostream& stream, bool is_newline)
	{
	#if 0
		SetConsoleTextAttribute(m_hConsole, DEFAULT_FOREGROUND
			| COMMON_LVB_GRID_HORIZONTAL * attributes[TOP]
			| COMMON_LVB_GRID_LVERTICAL * attributes[LEFT]
			| COMMON_LVB_GRID_RVERTICAL * attributes[RIGHT]);
	#endif

		// cursor
		moveCode(stream);

		// if nothing has changed do not modify the stream
		if (!m_cleared)
		{
			bool has_changed = false;
			for (size_t i = 0; i < ATTR_COUNT; i++)
				if (attributes[i] != last_attributes[i])
				{
					has_changed = true;
					break;
				}

			if (!has_changed && m_foreground != m_last_foreground)
				has_changed = true;

			if (!has_changed && m_background != m_last_background)
				has_changed = true;

			if (m_cleared)
				has_changed = true;

			if (!has_changed)
				return;
		}

		// colour string buffer
		char colour_buffer[4];

		// formatting
		stream << AR_ANSI_CSI;

		// TODO: no reason for clear of attributes when m_cleared is set

		if (attributes[ITALIC] && (!last_attributes[ITALIC] || m_cleared))
			stream << ";3";
		else if (!attributes[ITALIC] && (last_attributes[ITALIC] || m_cleared))
			stream << ";23";

		if (attributes[UNDERLINE] && (!last_attributes[UNDERLINE] || m_cleared))
			stream << ";4";
		else if (!attributes[UNDERLINE] && (last_attributes[UNDERLINE] || m_cleared))
			stream << ";24";

		if (attributes[BLINK] && (!last_attributes[BLINK] || m_cleared))
			stream << ";5";
		else if (!attributes[BLINK] && (last_attributes[BLINK] || m_cleared))
			stream << ";25";

		if (attributes[STRIKE] && (!last_attributes[STRIKE] || m_cleared))
			stream << ";9";
		else if (!attributes[STRIKE] && (last_attributes[STRIKE] || m_cleared))
			stream << ";29";

		// colour
		if (attributes[BOLD] != last_attributes[BOLD] || m_background != m_last_background || m_foreground != m_last_foreground || m_cleared || is_newline)
		{

			unsigned char red = m_foreground.red;
			unsigned char green = m_foreground.green;
			unsigned char blue = m_foreground.blue;

			if (attributes[BOLD])
			{
				red += AR_BOLD_DIFF;
				green += AR_BOLD_DIFF;
				blue += AR_BOLD_DIFF;

				red = red > m_foreground.red ? red : 255;
				green = green > m_foreground.green ? green : 255;
				blue = blue > m_foreground.blue ? blue : 255;
			}
			

			stream << ";38;2;";
			fixedToString<3>(red, colour_buffer);
			stream << colour_buffer << ';';
			fixedToString<3>(green, colour_buffer);
			stream << colour_buffer << ';';
			fixedToString<3>(blue, colour_buffer);
			stream << colour_buffer;

			stream << ";48;2;";
			fixedToString<3>(m_background.red, colour_buffer);
			stream << colour_buffer << ';';
			fixedToString<3>(m_background.green, colour_buffer);
			stream << colour_buffer << ';';
			fixedToString<3>(m_background.blue, colour_buffer);
			stream << colour_buffer;
		}

		stream << 'm';

		silentStore();
	}

	void AsciiAttr::silentStore()
	{
		last_attributes = attributes;
		m_last_foreground = m_foreground;
		m_last_background = m_background;

		m_cleared = false;
	}

	void AsciiAttr::setTitle(std::ostream& stream, const std::string& name)
	{
		stream << AR_ANSI_OSC << "0;" << name << '\a';
	}

	std::ostream& operator<<(std::ostream& stream, const Colour& c)
	{
		stream << std::setfill(' ') << std::setw(3) << (int)c.red << ' '
			<< std::setfill(' ') << std::setw(3) << (int)c.green << ' '
			<< std::setfill(' ') << std::setw(3) << (int)c.blue << ' '
			<< std::setfill(' ') << std::setw(3) << (int)c.alpha;
		return stream;
	}

	std::ostream& operator<<(std::ostream& stream, AsciiAttr& other)
	{
		other.ansiCode(stream);
		return stream;
	}
}