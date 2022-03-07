#pragma once

#include "Asciir/Core/Core.h"
#include "Asciir/Maths/Vertices.h"

namespace Asciir
{
	static constexpr size_t ATTR_COUNT = 8;

	typedef unsigned short ATTRI;

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

	static constexpr ATTRI BOLD = 0;
	static constexpr ATTRI ITALIC = 1;
	/// same as BOTTOM
	static constexpr ATTRI UNDERLINE = 2;
	/// @attention not supported on windows
	static constexpr ATTRI BLINK = 3;
	/// @attention not supported on windows
	static constexpr ATTRI STRIKE = 4;
	/// same as OVERLINED
	static constexpr ATTRI TOP = 7;
	/// same as TOP
	static constexpr ATTRI OVERLINED = 7;
	/// same as UNDERLINE
	static constexpr ATTRI BOTTOM = 2;
#ifdef AR_WIN
	/// @attention only supported on windows
	static constexpr ATTRI LEFT = 5;
	/// @attention only supported on windows
	static constexpr ATTRI RIGHT = 6;
#else
	/// @attention not supported on windows, for a windows alternative, set the TOP, BOTTOM, LEFT and RIGHT attributes for the same functionality.
	static constexpr ATTRI FRAMED = 5;
	/// @attention not supported on windows
	static constexpr ATTRI ENCIRCLED = 6;
#endif

	/// @brief the maximum size (in bytes) for a single ansi code that will enable / disable the specified attributes
	static constexpr size_t ATTR_MAX_SIZE = 48;

	/// @brief structure representing a 32 bit rgba colour value.
	struct Colour
	{
		unsigned char red, green, blue, alpha;

		/// @brief default constructor, creates a black colour with maximum alpha.
		Colour();
		/// @brief constructs a colour from the given channel values.
		Colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a = UCHAR_MAX);
		/// @brief constructs a grey scale colour value from the given grey value.
		Colour(unsigned char grey, unsigned char a = UCHAR_MAX);
		Colour(const Colour& other);

		/// @brief returns the inverse of the colour
		Colour inverse() const;

		/// @brief checks if all colour channels have the same value
		bool operator==(const Colour& other) const;
		/// @brief opposite of operator==()
		bool operator!=(const Colour& other) const;

		/// @brief blends the two colours taking the alpha value into account
		/// uses the current colour as the background and calculates the alpha result afterwards
		Colour& blend(const Colour& other);
		/// @brief blends the two colours taking the alpha value into account
		static Colour blend(const Colour& background, const Colour& colour);
	};

	/// @brief puts each channels value into the passed stream, seperated by spaces.
	std::ostream& operator<<(std::ostream& stream, const Colour& c);

	typedef Colour RGB32;

	/// @brief represents a 8 bit rgb value (256 possible colour values). each channel cannot have a value greater than 6
	/// this structure only represents the colour values in the range 16-231,
	/// for the range 0-15 values, use RGB4, and for the last range of 232-255, use GRAY8.
	struct RGB8
	{
		unsigned char red, green, blue;

		RGB8(unsigned char r, unsigned char g, unsigned char b);
		/// @brief converts the given RGB32 colour value to the closest RGB8 representation
		RGB8(const Colour& colour);
		RGB8();

		/// @brief get equivalent Colour structure, that represents the same colour as the current RGB8 instance
		Colour getColour();
		/// @see getColour()
		operator Colour();

		/// @brief returns the index in the terminal colour table that represents the current colour values.
		unsigned int getIndx();
	};

	/// @brief represents a 4 bit rgb.  
	/// each channel is either on or off.
	/// if intensity is 1, the resulting colour will be brighter.  
	/// the final RGB32 colour value is picked from a colour table.
	struct RGB4
	{
		bool red, green, blue, intensity;

		RGB4();
		RGB4(bool r, bool g, bool b, bool i = false);

		/// @brief get the RGB32 representation of the current RGB4 value.
		Colour getColour();
		/// @see getColour()
		operator Colour();
	};

	/// @brief represents a 8 bit grey scale colour value
	struct GRAY8
	{
		unsigned char grey;

		GRAY8(unsigned char g);
		GRAY8();

		/// @brief get the corresponding RGB32 colour value for the given GRAY8 colour.
		Colour getColour();
		operator Colour();
	};

	

	/// @brief class for storing and modifying the ansi attributes of an ascii character
	/// 
	/// also generates the corresponding ansi code that should be printed to the terminal in order to apply the attributes
	/// 
	/// The generated ansi codes assume no other attributes have been applied to the terminal inbetween modifications (not true if resuilt is of string type)
	/// 
	/// This means if an attribute contains the attributes (colour: Orange, Text: Bold) generates the ascii code, and outputs it to the terminal
	/// and is then later modified to contain the attributes (colour: Orange, Text: Underlined).
	/// The next call to ansiCode will remove the bold attribute and apply the underlined attribute whilst doing nothing to the colour, as it assumes it is still Orange.
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
		std::array<bool, ATTR_COUNT> last_attributes = { false };

		TermVert m_pos;

		bool m_should_move = false;
		bool m_cleared = true;

	public:
		/// @brief list of the attributes to be set / cleared on the next ansiCode() call.
		std::array<bool, ATTR_COUNT> attributes = { false };

		AsciiAttr() = default;
		virtual ~AsciiAttr();

		void setForeground(const Colour& colour);
		Colour getForeground();
		void setBackground(const Colour& colour);
		Colour getBackground();
		void setColour(const Colour& foreground, const Colour& background);

		/// @brief unsets all attributes and sets the foreground and background colour to their default colours.
		void clear();
		/// @brief unsets all attributes related to the format of the ascii character (everything except the colour)
		void clearFormat();
		/// @brief sets the colour to the default colour (white foreground, black background)
		void clearColour();

		/// @brief sets the passed attributes to the passed value.  
		/// same as:
		/// > attributes[attribute] = val;
		void setAttribute(const ATTRI& attribute, bool val);

		/// @brief moves the terminal cursor to the specified column and row in the terminal.
		void move(TermVert pos);

#ifdef AR_WIN

		
		void setBoxed(bool val);
		void setLR(bool val);
		void setTB(bool val);

#endif

		/// @brief returns the ansi code sequence that will set / unset the specified attributes.
		virtual std::string ansiCode() = 0;
		/// @brief appends return value of ansiCode(), to dst.
		virtual void ansiCode(std::string& dst) = 0;
		/// @brief puts the shortest possible ansi code sequence that will set / unset the specified attributes, taking into account the previous attributes set.
		/// 
		/// this function should always be used with the same stream, as it assumes the previous attributes put into the last stream, also are set in this stream.
		/// this also means this stream should only recieve ansicodes from this AsciiAttribute instance.
		/// remember to set the is_newline flag appropiatly.  
		///
		/// if a new stream is used, call clear() before using this function.
		///
		/// @param is_newline should be set to true if the stream has recieved a newline since the last call to ansiCode(std::ostream&, bool).
		virtual void ansiCode(std::ostream& stream, bool is_newline = false) = 0;

		/// @brief generates the ansi code that will move to the coord specified in move()
		/// @param dst the string that will recieve the move code
		void moveCode(std::string& dst);
		/// @brief same as moveCode(std::string&)
		/// @brief the ostream that will recieve the move code
		void moveCode(std::ostream& stream);


		/// @brief genereates the ansi coe that will change the name of the active console to the passed name, and puts it in to the specified stream.
		void setTitle(std::ostream& stream, const std::string& name);
	};

	/// @brief same as other.ansiCode(stream)
	std::ostream& operator<<(std::ostream& stream, AsciiAttr& other);
}
