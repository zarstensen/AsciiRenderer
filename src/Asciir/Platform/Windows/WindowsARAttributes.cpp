#include "arpch.h"
#include "Asciir/Rendering/AsciiAttributes.h"

#ifdef AR_WIN
namespace Asciir
{
	AsciiAttr::AsciiAttr()
		: m_hConsole(GetStdHandle(STD_OUTPUT_HANDLE))
	{
		// enable ansi code support
		DWORD dwMode = 0;
		GetConsoleMode(m_hConsole, &dwMode);
		SetConsoleMode(m_hConsole, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
		clearColor();
	}

	std::string AsciiAttr::ansiCode()
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

		if (m_should_move)
		{
			dst += AR_ANSIS_CSI;
			dst += std::to_string(m_pos.x + 1) + ',' + std::to_string(m_pos.y + 1) + 'H';

			m_should_move = false;
		}

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

	Coord AsciiAttr::terminalPos() const
	{
		CONSOLE_SCREEN_BUFFER_INFO console_info;
		GetConsoleScreenBufferInfo(m_hConsole, &console_info);

		long long x = console_info.srWindow.Top;
		long long y = console_info.srWindow.Left;

		return { x, y };
	}

	TermVert AsciiAttr::terminalSize() const
	{
		CONSOLE_SCREEN_BUFFER_INFO console_info;
		GetConsoleScreenBufferInfo(m_hConsole, &console_info);
		TInt width = console_info.srWindow.Right - console_info.srWindow.Left + 1;
		TInt height = console_info.srWindow.Bottom - console_info.srWindow.Top + 1;

		return { width, height };
	}

	TermVert AsciiAttr::maxTerminalSize() const
	{
		COORD max_size = GetLargestConsoleWindowSize(m_hConsole);

		return { max_size.X, max_size.Y };
	}
}
#endif
