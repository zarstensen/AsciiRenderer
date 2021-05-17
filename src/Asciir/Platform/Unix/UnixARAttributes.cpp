#include "arpch.h"
#include "Asciir/Rendering/AsciiAttributes.h"
#include "Asciir/Logging/Log.h"

namespace Asciir
{
	AsciiAttr::AsciiAttr()
	{
		// X11 initialization

		m_display = XOpenDisplay(NULL);
		m_window = DefaultRootWindow(m_display);
		int revert;

		XGetInputFocus(m_display, &m_focus_win, &revert);
		XSelectInput(m_display, m_focus_win, KeyPressMask | KeyReleaseMask | FocusChangeMask);

		// disable echo
		termios term;
		tcgetattr(STDIN_FILENO, &term);
		term.c_lflag |= ~ECHO;
		tcsetattr(STDIN_FILENO, TCSANOW, &term);

		clearColor();
	}

	AsciiAttr::~AsciiAttr()
	{
		XCloseDisplay(m_display);
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
		// cursor
		moveCode(dst);

		// formatting
		dst += AR_ANSIS_CSI;
		dst += "0";

		if(attributes[BOLD])
			dst += ";1";

		if (attributes[ITALIC])
			dst += ";3";

		if (attributes[UNDERLINE])
			dst += ";4";

		if (attributes[BLINK])
			dst += ";5";

		if (attributes[STRIKE])
			dst += ";9";

		// foreground color

		dst += ";38;2;";
		dst += std::to_string(m_foreground.red);
		dst += ";";
		dst += std::to_string(m_foreground.green);
		dst += ";";
		dst += std::to_string(m_foreground.blue);
		dst += ";";
		

		// background color
		dst += "48;2;";
		dst += std::to_string(m_background.red);
		dst += ";";
		dst += std::to_string(m_background.green);
		dst += ";";
		dst += std::to_string(m_background.blue);
		dst += 'm';
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

	Coord AsciiAttr::terminalPos() const
	{
		Coord pos;
		XWindowAttributes win_info;

		XGetWindowAttributes(m_display, m_window, &win_info);

		return { win_info.x, win_info.y };
	}

	TermVert AsciiAttr::terminalSize() const
	{
		winsize size;

		ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
		
		return TermVert(size.ws_col, size.ws_row);
	}

	TermVert AsciiAttr::maxTerminalSize() const
	{
		// no limits for maximum terminal size on unix systems (?)
		constexpr TInt max_int = std::numeric_limits<TInt>::max();
		return TermVert(max_int, max_int);
	}
}
