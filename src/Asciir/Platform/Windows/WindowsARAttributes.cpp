#include "arpch.h"
#include "WindowsARAttributes.h"
#include "Asciir/Rendering/TerminalRenderer.h"
#include "Asciir/Rendering/RenderConsts.h"

namespace Asciir
{
	// helper function for converting an integer to a string, with known maximum integer length, in order to avoid heap allocation
	template<size_t w, typename T>
	void fixedToString(T value, char* out)
	{
		snprintf(out, w + 1, "%d", value);
	}


	WinARAttr::WinARAttr()
		: m_hConsole(GetStdHandle(STD_OUTPUT_HANDLE))
	{
		clearColour();
	}

	WinARAttr::~WinARAttr()
	{
		
	}

	std::string WinARAttr::ansiCode()
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

	void WinARAttr::ansiCode(std::string& dst)
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
		dst += AR_ANSIS_CSI;
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

		if (attributes[BOLD])
		{
			unsigned char red = m_foreground.red + AR_BOLD_DIFF;
			unsigned char green = m_foreground.green + AR_BOLD_DIFF;
			unsigned char blue = m_foreground.blue + AR_BOLD_DIFF;

			red = red > m_foreground.red ? red : 255;
			green = green > m_foreground.green ? green : 255;
			blue = blue > m_foreground.blue ? blue : 255;

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
		}
		else
		{
			dst += ";38;2;";
			fixedToString<3>(m_foreground.red, colour_buffer);
			dst += colour_buffer;
			dst += ";";
			fixedToString<3>(m_foreground.green, colour_buffer);
			dst += colour_buffer;
			dst += ";";
			fixedToString<3>(m_foreground.blue, colour_buffer);
			dst += colour_buffer;
			dst += ";";
		}

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

	void WinARAttr::ansiCode(std::ostream& stream, bool is_newline)
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

		// colour string buffer
		char colour_buffer[4];

		// formatting
		stream << AR_ANSIS_CSI;

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

		// foreground colour
		// background colour needs to be updated if the foreground colour is changed
		if (attributes[BOLD] && ((!last_attributes[BOLD] || m_foreground != m_last_foreground) || m_cleared || is_newline))
		{
			unsigned char red = m_foreground.red + AR_BOLD_DIFF;
			unsigned char green = m_foreground.green + AR_BOLD_DIFF;
			unsigned char blue = m_foreground.blue + AR_BOLD_DIFF;

			red = red > m_foreground.red ? red : 255;
			green = green > m_foreground.green ? green : 255;
			blue = blue > m_foreground.blue ? blue : 255;

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
		else if (m_foreground != m_last_foreground || m_cleared || is_newline)
		{
			stream << ";38;2;";
			fixedToString<3>(m_foreground.red, colour_buffer);
			stream << colour_buffer << ';';
			fixedToString<3>(m_foreground.green, colour_buffer);
			stream << colour_buffer << ';';
			fixedToString<3>(m_foreground.blue, colour_buffer);
			stream << colour_buffer;

			stream << ";48;2;";
			fixedToString<3>(m_background.red, colour_buffer);
			stream << colour_buffer << ';';
			fixedToString<3>(m_background.green, colour_buffer);
			stream << colour_buffer << ';';
			fixedToString<3>(m_background.blue, colour_buffer);
			stream << colour_buffer;
		}
		else if (m_background != m_last_background || m_cleared || is_newline)
		{
			// only background colour
			stream << ";48;2;";
			fixedToString<3>(m_background.red, colour_buffer);
			stream << colour_buffer << ';';
			fixedToString<3>(m_background.green, colour_buffer);
			stream << colour_buffer << ';';
			fixedToString<3>(m_background.blue, colour_buffer);
			stream << colour_buffer;
		}

		stream << 'm';
		last_attributes = attributes;
		m_last_foreground = m_foreground;
		m_last_background = m_background;

		m_cleared = false;
	}

	
}