#include "arpch.h"
#include "AsciiAttributes.h"
#include "TerminalRenderer.h"
#include "RenderConsts.h"
#include "Asciir/Logging/Log.h"

namespace Asciir
{
	Color::Color()
		: red(0), green(0), blue(0), alpha(UCHAR_MAX)
	{}

	Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
		: red(r), green(g), blue(b), alpha(a)
	{}

	Color::Color(unsigned char gray, unsigned char a)
		: red(gray), green(gray), blue(gray), alpha(a)
	{}

	Color::Color(const Color& other)
		: red(other.red), green(other.green), blue(other.blue), alpha(other.alpha)
	{}

	Color Color::inverse() const
	{
		return Color(UCHAR_MAX - red, UCHAR_MAX - green, UCHAR_MAX - blue);
	}

	bool Color::operator==(const Color& other) const
	{
		return red == other.red && green == other.green && blue == other.blue;
	}
	bool Color::operator!=(const Color& other) const
	{
		return !operator==(other);
	}
	bool Color::operator<(const Color& other) const
	{
		return (red + green + blue) / 3 < (other.red + other.green + other.blue) / 3;
	}
	bool Color::operator>(const Color& other) const
	{
		return (red + green + blue) / 3 > (other.red + other.green + other.blue) / 3;
	}
	bool Color::operator<=(const Color& other) const
	{
		return (red + green + blue) / 3 <= (other.red + other.green + other.blue) / 3;
	}
	bool Color::operator>=(const Color& other) const
	{
		return (red + green + blue) / 3 >= (other.red + other.green + other.blue) / 3;
	}

	Color& Color::blend(const Color& other)
	{
		Real foreground_alpha = (Real)other.alpha / UCHAR_MAX;

		red =	unsigned char(foreground_alpha * other.red + (1 - foreground_alpha) * (Real) red);
		green = unsigned char(foreground_alpha * other.green + (1 - foreground_alpha) * (Real) green);
		blue =	unsigned char(foreground_alpha * other.blue + (1 - foreground_alpha) * (Real) blue);
		alpha = unsigned char(alpha + (1 - (Real) alpha / UCHAR_MAX) * foreground_alpha);

		return *this;
	}

	Color Color::blend(const Color& background, const Color& color)
	{
		Color result = background;
		result.blend(color);
		return result;
	}

	RGB8::RGB8(unsigned char r, unsigned char g, unsigned char b)
	{
		red = r;
		green = g;
		blue = b;

		AR_ASSERT_MSG(red < 6, "Red color value must be less than 6. value: ", red);
		AR_ASSERT_MSG(green < 6, "Green color value must be less than 6. value: ", green);
		AR_ASSERT_MSG(blue < 6, "Blue color value must be less than 6. value: ", blue);
	}

	RGB8::RGB8()
		: red(0), green(0), blue(0)
	{}

	Color RGB8::getColor()
	{
		return Color(CGRADIENT16[red], CGRADIENT16[green], CGRADIENT16[blue]);
	}

	RGB8::operator Color()
	{
		return getColor();
	}

	GRAY8::GRAY8(unsigned char g)
		: gray(g)
	{
		AR_ASSERT_MSG(gray < 24, "Gray value must be less than 24. value: ", gray);
	}

	GRAY8::GRAY8()
		: gray(0)
	{}

	Color GRAY8::getColor()
	{
		// converts gray color from range (0-23) to an 8 bit value (8-238)
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
				AR_ASSERT_MSG(false, "Invalid color");
				return {};
		}
		
	}

	RGB4::operator Color()
	{
		return getColor();
	}

	// constructor and destructor is platform dependent

	AsciiAttr::~AsciiAttr() {}

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
		clearColor();
		m_cleared = true;
	}

	void AsciiAttr::clearFormat()
	{
		memset(attributes.data(), false, ATTR_COUNT);
	}
	
	void AsciiAttr::clearColor()
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
			dst += AR_ANSIS_CSI;
			dst += std::to_string(m_pos.y + 1) + ';' + std::to_string(m_pos.x + 1) + 'H';

			m_should_move = false;
		}
	}

	void AsciiAttr::moveCode(std::ostream& stream)
	{
		if (m_should_move)
		{
			stream << AR_ANSIS_CSI;
			stream << std::to_string(m_pos.y + 1) << ';' << std::to_string(m_pos.x + 1) << 'H';

			m_should_move = false;
		}
	}

	void AsciiAttr::moveCode(TerminalRenderer& dst)
	{
		if (m_should_move)
		{
			dst.pushBuffer(AR_ANSIS_CSI);
			dst.pushBuffer(std::to_string(m_pos.y + 1) + ';' + std::to_string(m_pos.x + 1) + 'H');

			m_should_move = false;
		}
	}

	void AsciiAttr::setTitle(const std::string& name)
	{
		std::cout << AR_ANSIS_OSC << "0;" << name << '\a';
	}

	std::ostream& operator<<(std::ostream& stream, const Color& c)
	{
		stream	<< std::setfill(' ') << std::setw(3) << (int) c.red		<< ' '
				<< std::setfill(' ') << std::setw(3) << (int) c.green	<< ' '
				<< std::setfill(' ') << std::setw(3) << (int) c.blue	<< ' '
				<< std::setfill(' ') << std::setw(3) << (int) c.alpha;
		return stream;
	}

	std::ostream& operator<<(std::ostream& stream, AsciiAttr& other)
	{
		other.ansiCode(stream);
		return stream;
	}
}