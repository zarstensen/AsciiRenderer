#pragma once

#include "Asciir/Core/Core.h"
#include "Asciir/Math/Vertices.h"

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

	struct Color
	{
		unsigned char red, green, blue;

		Color();
		Color(unsigned char r, unsigned char g, unsigned char b);
		Color(unsigned char gray);
		Color(const Color& other);

		Color inverse() const;

		bool operator==(const Color& other) const;
		bool operator!=(const Color& other) const;
		bool operator<(const Color& other) const;
		bool operator>(const Color& other) const;
		bool operator<=(const Color& other) const;
		bool operator>=(const Color& other) const;
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

	class TerminalRenderer;

	class AsciiAttr
	{
	protected:
		Color m_foreground;
		Color m_background;

		Color m_last_foreground;
		Color m_last_background;

		std::array<bool, ATTR_COUNT> last_attributes;

		TermVert m_pos;

		bool m_should_move = false;
		bool m_cleared = true;

	public:

		std::array<bool, ATTR_COUNT> attributes;

		AsciiAttr() = default;
		virtual ~AsciiAttr();

		void setForeground(const Color& color);
		Color getForeground();
		void setBackground(const Color& color);
		Color getBackground();
		void setColor(const Color& foreground, const Color& background);

		void clear();
		void clearFormat();
		void clearColor();

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
		virtual void ansiCode(TerminalRenderer& dst, bool is_newline = false) = 0;
		
		void moveCode(std::string& dst);
		void moveCode(std::ostream& stream);
		void moveCode(TerminalRenderer& dst);


		void setTitle(const std::string& name);

		virtual Coord terminalPos() const = 0;
		virtual TermVert terminalSize() const = 0;
		virtual TermVert maxTerminalSize() const = 0;
		virtual Size2D fontSize() const = 0;
	};

	std::ostream& operator<<(std::ostream& stream, AsciiAttr& other);
}
