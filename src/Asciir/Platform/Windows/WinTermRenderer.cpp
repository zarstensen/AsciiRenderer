#include "WinTermRenderer.h"

#include "Asciir/Core/Application.h"

#include <Windows.h>

namespace Asciir
{
namespace TRInterface
{
	WinTerminalRenderer::WinTerminalRenderer(const WinTerminalRenderer::TerminalProps& props)
		: TerminalRendererInterface(props), m_console(GetStdHandle(STD_OUTPUT_HANDLE))
	{
		// enable ansi code support
		AR_WIN_VERIFY(GetConsoleMode(m_console, &m_fallback_console_mode));
		AR_WIN_VERIFY(SetConsoleMode(m_console, m_fallback_console_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING));
		

		// use alternate screen buffer
		// makes sure the text on the console before the application was started is not modified by the program.
		// also makes sure any text never is scrolled outside the console window.
		// this in turn means that console mouse events always report the correct y value.
		std::cout << AR_ANSIS_CSI << "?1049h";

		// enable UTF8 codepage
		m_fallback_codepage = GetConsoleOutputCP();
		AR_WIN_VERIFY(SetConsoleOutputCP(CP_UTF8));

		initRenderer(props);
	}

	WinTerminalRenderer::~WinTerminalRenderer()
	{
		// go back to normal screen buffer
		std::cout << AR_ANSIS_CSI << "?1049l";

		// reset console mode
		AR_WIN_VERIFY(SetConsoleMode(m_console, m_fallback_console_mode));
		AR_WIN_VERIFY(SetConsoleOutputCP(m_fallback_codepage));
	}
	
	TermVert WinTerminalRenderer::termSize() const
	{
		return ARApp::getApplication()->getTermEvtHandler().getEvtListener().winLastTermSize();
	}

	TermVert WinTerminalRenderer::maxSize() const
	{
		COORD max_size = GetLargestConsoleWindowSize(m_console);

		return { max_size.X, max_size.Y };
	}
	
	Coord WinTerminalRenderer::pos() const
	{
		RECT pos;
		// TODO: only call GetConsoleWindow once and store it?
		AR_WIN_VERIFY(GetWindowRect(GetConsoleWindow(), &pos));

		long long x = pos.left;
		long long y = pos.top;

		return { (Real)x, (Real)y };
	}

	Size2D WinTerminalRenderer::fontSize() const
	{
		CONSOLE_FONT_INFO f_info;

		AR_WIN_VERIFY(GetCurrentConsoleFont(m_console, false, &f_info));

		return { (size_t)f_info.dwFontSize.X, (size_t)f_info.dwFontSize.Y };
	}

	bool WinTerminalRenderer::isFocused() const
	{
		return GetForegroundWindow() == GetConsoleWindow();
	}
}
}
