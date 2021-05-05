#include "arpch.h"
#include "AsciiAttributes.h"
#include "RenderConsts.h"
#include "Asciir/Logging/Log.h"

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
				return NULL;
		}
		
	}

	RGB4::operator Color()
	{
		return getColor();
	}

	// constructor is platform dependent

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

	void AsciiAttr::setTitle(const std::string& name)
	{
		std::cout << AR_ANSIS_OSC << "0;" << name << AR_ANSIS_CSI;
	}

	std::ostream& operator<<(std::ostream& stream, AsciiAttr& other)
	{
		other.ansiCode(stream);
		return stream;
	}
}