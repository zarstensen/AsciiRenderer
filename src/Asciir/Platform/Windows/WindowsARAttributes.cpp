#include "arpch.h"
#include "WindowsARAttributes.h"
#include "Asciir/Rendering/RenderConsts.h"

namespace Asciir
{
	WinARAttr::WinARAttr()
		: m_hConsole(GetStdHandle(STD_OUTPUT_HANDLE))
	{
		// enable ansi code support
		DWORD dwMode = 0;
		GetConsoleMode(m_hConsole, &dwMode);
		SetConsoleMode(m_hConsole, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
		clearColor();
	}

	WinARAttr::~WinARAttr() {}

	std::string WinARAttr::ansiCode()
	{
		size_t size = 3 + (5 + 4 * 3) * 2;

		for (size_t i = 1; i < 5; i++)
		{
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

		// foreground color

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

		// background color
		dst += "48;2;";
		dst += std::to_string(m_background.red);
		dst += ";";
		dst += std::to_string(m_background.green);
		dst += ";";
		dst += std::to_string(m_background.blue);
		dst += 'm';
	}

	void WinARAttr::ansiCode(std::ostream& stream, bool is_newline)
	{

		#if 0
		SetConsoleTextAttribute(m_hConsole, DEFAULT_FOREGROUND
			| COMMON_LVB_GRID_HORIZONTAL * attributes[TOP]
			| COMMON_LVB_GRID_LVERTICAL * attributes[LEFT]
			| COMMON_LVB_GRID_RVERTICAL * attributes[RIGHT]);
		#endif

		// if nothing is change do not modify the stream
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

		// foreground color
		// background color needs to be updated if the foreground color is changed
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
			// only background color
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

	void WinARAttr::ansiCode(TerminalRender& dst, bool is_newline)
	{

		#if 0
		SetConsoleTextAttribute(m_hConsole, DEFAULT_FOREGROUND
			| COMMON_LVB_GRID_HORIZONTAL * attributes[TOP]
			| COMMON_LVB_GRID_LVERTICAL * attributes[LEFT]
			| COMMON_LVB_GRID_RVERTICAL * attributes[RIGHT]);
		#endif

		// if nothing is change do not modify the stream
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

		// formatting
		dst << AR_ANSIS_CSI;

		if (attributes[ITALIC] && (!last_attributes[ITALIC] || m_cleared))
			dst << ";3";
		else if (!attributes[ITALIC] && (last_attributes[ITALIC] || m_cleared))
			dst << ";23";

		if (attributes[UNDERLINE] && (!last_attributes[UNDERLINE] || m_cleared))
			dst << ";4";
		else if (!attributes[UNDERLINE] && (last_attributes[UNDERLINE] || m_cleared))
			dst << ";24";

		if (attributes[BLINK] && (!last_attributes[BLINK] || m_cleared))
			dst << ";5";
		else if (!attributes[BLINK] && (last_attributes[BLINK] || m_cleared))
			dst << ";25";

		if (attributes[STRIKE] && (!last_attributes[STRIKE] || m_cleared))
			dst << ";9";
		else if (!attributes[STRIKE] && (last_attributes[STRIKE] || m_cleared))
			dst << ";29";

		// foreground color
		// background color needs to be updated if the foreground color is changed
		if (attributes[BOLD] && ((!last_attributes[BOLD] || m_foreground != m_last_foreground) || m_cleared || is_newline))
		{
			unsigned char red = m_foreground.red + AR_BOLD_DIFF;
			unsigned char green = m_foreground.green + AR_BOLD_DIFF;
			unsigned char blue = m_foreground.blue + AR_BOLD_DIFF;

			red = red > m_foreground.red ? red : 255;
			green = green > m_foreground.green ? green : 255;
			blue = blue > m_foreground.blue ? blue : 255;

			dst << ";38;2;";
			dst << std::to_string(red);
			dst << ";";
			dst << std::to_string(green);
			dst << ";";
			dst << std::to_string(blue);

			dst << ";48;2;";
			dst << std::to_string(m_background.red);
			dst << ';';
			dst << std::to_string(m_background.green);
			dst << ';';
			dst << std::to_string(m_background.blue);
		}
		else if (m_foreground != m_last_foreground || m_cleared || is_newline)
		{
			dst << ";38;2;";
			dst << std::to_string(m_foreground.red);
			dst << ";";
			dst << std::to_string(m_foreground.green);
			dst << ";";
			dst << std::to_string(m_foreground.blue);

			dst << ";48;2;";
			dst << std::to_string(m_background.red);
			dst << ';';
			dst << std::to_string(m_background.green);
			dst << ';';
			dst << std::to_string(m_background.blue);
		}
		else if (m_background != m_last_background || m_cleared || is_newline)
		{
			// only background color
			dst << ";48;2;";
			dst << std::to_string(m_background.red);
			dst << ';';
			dst << std::to_string(m_background.green);
			dst << ';';
			dst << std::to_string(m_background.blue);
		}

		dst << 'm';
		last_attributes = attributes;
		m_last_foreground = m_foreground;
		m_last_background = m_background;

		m_cleared = false;
	}

	Coord WinARAttr::terminalPos() const
	{
		CONSOLE_SCREEN_BUFFER_INFO console_info;
		GetConsoleScreenBufferInfo(m_hConsole, &console_info);

		long long x = console_info.srWindow.Top;
		long long y = console_info.srWindow.Left;

		return { x, y };
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
}
