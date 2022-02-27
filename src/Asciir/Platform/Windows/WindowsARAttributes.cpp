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
		// enable ansi code support
		AR_WIN_VERIFY(GetConsoleMode(m_hConsole, &m_fallback_mode));
		AR_WIN_VERIFY(SetConsoleMode(m_hConsole, m_fallback_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING));
		clearColour();

		// use alternate screen buffer
		// makes sure the text on the console before the application was started is not modified by the program.
		// also makes sure any text never is scrolled outside the console window.
		// this in turn means that console mouse events always report the correct y value.
		std::cout << AR_ANSIS_CSI << "?1049h";

		// enable UTF8 codepage
		m_fallback_cp = GetConsoleOutputCP();
		AR_WIN_VERIFY(SetConsoleOutputCP(CP_UTF8));
	}

	WinARAttr::~WinARAttr()
	{
		// go back to normal screen buffer
		std::cout << AR_ANSIS_CSI << "?1049l";

		// reset console mode
		AR_WIN_VERIFY(SetConsoleMode(m_hConsole, m_fallback_mode));
		AR_WIN_VERIFY(SetConsoleOutputCP(m_fallback_cp));
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
		// cursor
		moveCode(stream);

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
			stream << std::to_string(red);
			stream << ";";
			stream << std::to_string(green);
			stream << ";";
			stream << std::to_string(blue);

			stream << ";48;2;";
			stream << std::to_string(m_background.red);
			stream << ';';
			stream << std::to_string(m_background.green);
			stream << ';';
			stream << std::to_string(m_background.blue);
		}
		else if (m_foreground != m_last_foreground || m_cleared || is_newline)
		{
			stream << ";38;2;";
			stream << std::to_string(m_foreground.red);
			stream << ";";
			stream << std::to_string(m_foreground.green);
			stream << ";";
			stream << std::to_string(m_foreground.blue);

			stream << ";48;2;";
			stream << std::to_string(m_background.red);
			stream << ';';
			stream << std::to_string(m_background.green);
			stream << ';';
			stream << std::to_string(m_background.blue);
		}
		else if (m_background != m_last_background || m_cleared || is_newline)
		{
			// only background colour
			stream << ";48;2;";
			stream << std::to_string(m_background.red);
			stream << ';';
			stream << std::to_string(m_background.green);
			stream << ';';
			stream << std::to_string(m_background.blue);
		}

		stream << 'm';
		last_attributes = attributes;
		m_last_foreground = m_foreground;
		m_last_background = m_background;

		m_cleared = false;
	}

	void WinARAttr::ansiCode(TerminalRenderer& dst, bool is_newline)
	{
#if 0
		SetConsoleTextAttribute(m_hConsole, DEFAULT_FOREGROUND
			| COMMON_LVB_GRID_HORIZONTAL * attributes[TOP]
			| COMMON_LVB_GRID_LVERTICAL * attributes[LEFT]
			| COMMON_LVB_GRID_RVERTICAL * attributes[RIGHT]);
#endif

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
		// cursor
		moveCode(dst);

		// colour string buffer
		char colour_buffer[4];

		// formatting
		dst.pushBuffer(AR_ANSIS_CSI);

		if (attributes[ITALIC] && (!last_attributes[ITALIC] || m_cleared))
			dst.pushBuffer(";3");
		else if (!attributes[ITALIC] && (last_attributes[ITALIC] || m_cleared))
			dst.pushBuffer(";23");

		if (attributes[UNDERLINE] && (!last_attributes[UNDERLINE] || m_cleared))
			dst.pushBuffer(";4");
		else if (!attributes[UNDERLINE] && (last_attributes[UNDERLINE] || m_cleared))
			dst.pushBuffer(";24");

		if (attributes[BLINK] && (!last_attributes[BLINK] || m_cleared))
			dst.pushBuffer(";5");
		else if (!attributes[BLINK] && (last_attributes[BLINK] || m_cleared))
			dst.pushBuffer(";25");

		if (attributes[STRIKE] && (!last_attributes[STRIKE] || m_cleared))
			dst.pushBuffer(";9");
		else if (!attributes[STRIKE] && (last_attributes[STRIKE] || m_cleared))
			dst.pushBuffer(";29");

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

			dst.pushBuffer(";38;2;");
			fixedToString<3>(red, colour_buffer);
			dst.pushBuffer(colour_buffer);
			dst.pushBuffer(';');
			fixedToString<3>(green, colour_buffer);
			dst.pushBuffer(colour_buffer);
			dst.pushBuffer(';');
			fixedToString<3>(blue, colour_buffer);
			dst.pushBuffer(colour_buffer);

			dst.pushBuffer(";48;2;");
			fixedToString<3>(m_background.red, colour_buffer);
			dst.pushBuffer(colour_buffer);
			dst.pushBuffer(';');
			fixedToString<3>(m_background.green, colour_buffer);
			dst.pushBuffer(colour_buffer);
			dst.pushBuffer(';');
			fixedToString<3>(m_background.blue, colour_buffer);
			dst.pushBuffer(colour_buffer);
		}
		else if (m_foreground != m_last_foreground || m_cleared || is_newline)
		{
			dst.pushBuffer(";38;2;");
			fixedToString<3>(m_foreground.red, colour_buffer);
			dst.pushBuffer(colour_buffer);
			dst.pushBuffer(';');
			fixedToString<3>(m_foreground.green, colour_buffer);
			dst.pushBuffer(colour_buffer);
			dst.pushBuffer(';');
			fixedToString<3>(m_foreground.blue, colour_buffer);
			dst.pushBuffer(colour_buffer);

			dst.pushBuffer(";48;2;");
			fixedToString<3>(m_background.red, colour_buffer);
			dst.pushBuffer(colour_buffer);
			dst.pushBuffer(';');
			fixedToString<3>(m_background.green, colour_buffer);
			dst.pushBuffer(colour_buffer);
			dst.pushBuffer(';');
			fixedToString<3>(m_background.blue, colour_buffer);
			dst.pushBuffer(colour_buffer);
		}
		else if (m_background != m_last_background || m_cleared || is_newline)
		{
			// only background colour
			dst.pushBuffer(";48;2;");
			fixedToString<3>(m_background.red, colour_buffer);
			dst.pushBuffer(colour_buffer);
			dst.pushBuffer(';');
			fixedToString<3>(m_background.green, colour_buffer);
			dst.pushBuffer(colour_buffer);
			dst.pushBuffer(';');
			fixedToString<3>(m_background.blue, colour_buffer);
			dst.pushBuffer(colour_buffer);
		}

		dst.pushBuffer('m');
		last_attributes = attributes;
		m_last_foreground = m_foreground;
		m_last_background = m_background;

		m_cleared = false;
	}

	Coord WinARAttr::terminalPos() const
	{
		RECT pos;
		AR_WIN_VERIFY(GetWindowRect(GetConsoleWindow(), &pos));

		long long x = pos.left;
		long long y = pos.top;

		return { (Real)x, (Real)y };
	}

	TermVert WinARAttr::terminalSize() const
	{
		CONSOLE_SCREEN_BUFFER_INFO console_info;
		GetConsoleScreenBufferInfo(m_hConsole, &console_info);
		TInt width = console_info.srWindow.Right - console_info.srWindow.Left + 1;
		TInt height = console_info.srWindow.Bottom - console_info.srWindow.Top + 1;

		return { width, height };
	}

	TermVert WinARAttr::maxTerminalSize() const
	{
		COORD max_size = GetLargestConsoleWindowSize(m_hConsole);

		return { max_size.X, max_size.Y };
	}

	Size2D WinARAttr::fontSize() const
	{
		CONSOLE_FONT_INFO f_info;

		AR_WIN_VERIFY(GetCurrentConsoleFont(m_hConsole, false, &f_info));

		return { (size_t)f_info.dwFontSize.X, (size_t)f_info.dwFontSize.Y };
	}
}