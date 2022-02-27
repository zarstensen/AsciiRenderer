#pragma once

#include "Asciir/Core/Core.h"
#include "Asciir/Maths/Vertices.h"

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

	static constexpr size_t ATTR_MAX_SIZE = 48;

	struct Colour
	{
		unsigned char red, green, blue, alpha;

		Colour();
		Colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a = UCHAR_MAX);
		Colour(unsigned char gray, unsigned char a = UCHAR_MAX);
		Colour(const Colour& other);

		Colour inverse() const;

		bool operator==(const Colour& other) const;
		bool operator!=(const Colour& other) const;
		bool operator<(const Colour& other) const;
		bool operator>(const Colour& other) const;
		bool operator<=(const Colour& other) const;
		bool operator>=(const Colour& other) const;

		// blends the two Colours taking the alpha value into account
		// uses the current Colour as the background and calculates the alpha result afterwards
		Colour& blend(const Colour& other);
		// blends the two Colours taking the alpha value into account
		static Colour blend(const Colour& background, const Colour& Colour);
	};

	std::ostream& operator<<(std::ostream& stream, const Colour& c);

	typedef Colour RGB24;

	struct RGB8
	{
		unsigned char red, green, blue;

		RGB8(unsigned char r, unsigned char g, unsigned char b);
		RGB8();

		Colour getColour();
		operator Colour();
	};

	struct RGB4
	{
		bool red, green, blue, intensity;

		RGB4();
		RGB4(bool r, bool g, bool b, bool i = false);

		Colour getColour();
		operator Colour();
	};

	struct GRAY8
	{
		unsigned char gray;

		GRAY8(unsigned char g);
		GRAY8();

		Colour getColour();
		operator Colour();
	};

	/*
	* windows does not support
	*
	* Blink
	* Strike
	* Framed
	* Encircled
	* Overlined (= Top)
	*
	* only windows supports
	*
	* Left
	* Right
	* Top (= Overlined)
	* Bottom
	*
	* All of the above = Framed
	*/

	class TerminalRenderer;

	/// @brief class for storing and modifying the ansi attributes of an ascii character
	/// 
	/// also generates the corresponding ansi code that should be printed to the terminal in order to apply the attributes
	/// 
	/// The generated ansi codes assume no other attributes have been applied to the terminal inbetween modifications (not true if resuilt is of string type)
	/// 
	/// This means if an attribute contains the attributes (Colour: Orange, Text: Bold) generates the ascii code, and outputs it to the terminal
	/// and is then later modified to contain the attributes (Colour: Orange, Text: Underlined).
	/// The next call to ansiCode will remove the bold attribute and apply the underlined attribute whilst doing nothing to the Colour, as it assumes it is still Orange.
	/// 
	/// a call to clear() will assume unknown modifications have happened, and generate the complete ansi code applying all the attributes whilst clearing all other.
	/// 
	/// @attention only some information is preserved on a newline in streams, so the is_newline parameter must be set accordingly in order to accomidate for this, when calling ansiCode().
	///
	class AsciiAttr
	{
	protected:
		Colour m_foreground;
		Colour m_background;

		Colour m_last_foreground;
		Colour m_last_background;

		// used for calculating the only needed modifications to the next ansi code
		std::array<bool, ATTR_COUNT> last_attributes;

		TermVert m_pos;

		bool m_should_move = false;
		bool m_cleared = true;

	public:

		std::array<bool, ATTR_COUNT> attributes;

		AsciiAttr() = default;
		virtual ~AsciiAttr();

		void setForeground(const Colour& Colour);
		Colour getForeground();
		void setBackground(const Colour& Colour);
		Colour getBackground();
		void setColour(const Colour& foreground, const Colour& background);

		void clear();
		void clearFormat();
		void clearColour();

		void setAttribute(const ATTRI& attribute, bool val);

		void move(TermVert pos);

#ifdef AR_WIN

		void setBoxed(bool val);
		void setLR(bool val);
		void setTB(bool val);

#endif

		virtual std::string ansiCode() = 0;
		virtual void ansiCode(std::string& dst) = 0;
		virtual void ansiCode(std::ostream& stream, bool is_newline = false) = 0;

		// TODO: no reason to have this here, just make sure terminal renderer has a stream input class.
		virtual void ansiCode(TerminalRenderer& dst, bool is_newline = false) = 0;

		void moveCode(std::string& dst);
		void moveCode(std::ostream& stream);

		// TODO. same as ansiCode(TerminalRenderer)
		void moveCode(TerminalRenderer& dst);

		void setTitle(const std::string& name);

		virtual Coord terminalPos() const = 0;
		virtual TermVert terminalSize() const = 0;
		virtual TermVert maxTerminalSize() const = 0;
		virtual Size2D fontSize() const = 0;
	};

	std::ostream& operator<<(std::ostream& stream, AsciiAttr& other);
}
