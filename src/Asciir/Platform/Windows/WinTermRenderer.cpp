#include "WinTermRenderer.h"

#include "arpch.h"

#include <Windows.h>
#include <ChrTrc.h>

#include "Asciir/Core/Application.h"


namespace Asciir
{
namespace TRInterface
{
	// ============ TermRendererBuffer ============

	TermRendererBuffer::TermRendererBuffer(size_t buffer_size)
		: m_console_hwin(GetConsoleWindow())
	{
		m_buffer.reserve(buffer_size);

		// allocate console buffers
		m_hconsole_display = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		m_hconsole_writable = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

		DWORD mode;
		GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &mode);

		AR_WIN_VERIFY(SetConsoleMode(m_hconsole_writable, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING));
		AR_WIN_VERIFY(SetConsoleMode(m_hconsole_display, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING));

		AR_WIN_VERIFY(SetConsoleActiveScreenBuffer(m_hconsole_display));

		const char* disable_scroll = "\x1b[0;0r"; // https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences#scrolling-margins

		for(HANDLE h: getCBuffers())
			WriteFile(h, disable_scroll, (DWORD) strlen(disable_scroll), NULL, NULL);
	}

	std::streamsize TermRendererBuffer::xsputn(const std::streambuf::char_type* s, std::streamsize count)
	{
		if (count + m_buffer.size() > m_buffer.capacity())
			sync();

		if ((size_t)count > m_buffer.capacity())
		{
			AR_WIN_VERIFY(WriteFile(m_hconsole_writable, m_buffer.c_str(), (DWORD) m_buffer.size(), NULL, NULL));
			AR_WIN_VERIFY(WriteFile(m_hconsole_display, m_buffer.c_str(), (DWORD) m_buffer.size(), NULL, NULL));
		}
		else
		{
			m_buffer += s;
		}

		return count;
	}

	std::streambuf::int_type TermRendererBuffer::overflow(std::streambuf::int_type ch)
	{
		if (ch != std::streambuf::traits_type::eof())
		{
			if (m_buffer.size() == m_buffer.capacity())
				sync();

			m_buffer += (char)ch;

			return 0;
		}
		else
		{
			return -1;
		}
	}

	// this should be used sparingly in the windows implementation, as it write to both the writable and display buffer in order to sync the data.
	int TermRendererBuffer::sync()
	{
		AR_WIN_VERIFY(WriteFile(m_hconsole_writable, m_buffer.c_str(), (DWORD) m_buffer.size(), NULL, NULL));
		AR_WIN_VERIFY(WriteFile(m_hconsole_display, m_buffer.c_str(), (DWORD) m_buffer.size(), NULL, NULL));
		m_buffer.clear();

		return 0;
	}

	void TermRendererBuffer::swapCBuffer()
	{
		{
			CT_MEASURE_N("BWRITE 1");
			AR_WIN_VERIFY(WriteFile(m_hconsole_writable, m_buffer.c_str(), (DWORD)m_buffer.size(), NULL, NULL));
		}

		std::swap(m_hconsole_writable, m_hconsole_display);

		{
			CT_MEASURE_N("BSWAP");
			AR_WIN_VERIFY(SetConsoleActiveScreenBuffer(m_hconsole_display));
		}

		{
			CT_MEASURE_N("BWRITE 2");
			AR_WIN_VERIFY(WriteFile(m_hconsole_writable, m_buffer.c_str(), (DWORD)m_buffer.size(), NULL, NULL));
		}

		m_buffer.clear();
	}

	// ============ WinTerminalRenderer ============

	WinTerminalRenderer::WinTerminalRenderer(const WinTerminalRenderer::TerminalProps& props)
		: TerminalRendererInterface(props), m_buffer(props.buffer_size), m_console(GetStdHandle(STD_OUTPUT_HANDLE)), m_console_hwin(GetConsoleWindow())
	{
		// enable ansi code support
		AR_WIN_VERIFY(GetConsoleMode(m_console, &m_fallback_console_mode));
		AR_WIN_VERIFY(SetConsoleMode(m_console, m_fallback_console_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING));
		

		// use alternate screen buffer
		// makes sure the text on the console before the application was started is not modified by the program.
		// also makes sure any text never is scrolled outside the console window.
		// this in turn means that console mouse events always report the correct y value.

		// enable UTF8 codepage
		m_fallback_codepage = { GetConsoleOutputCP(), GetConsoleCP() };
		AR_WIN_VERIFY(SetConsoleOutputCP(CP_UTF8));
		AR_WIN_VERIFY(SetConsoleCP(CP_UTF8));

		initRenderer(props);
	}

	WinTerminalRenderer::~WinTerminalRenderer()
	{

		// reset console mode
		//AR_WIN_VERIFY(SetConsoleMode(m_console, m_fallback_console_mode));
		AR_WIN_VERIFY(SetConsoleOutputCP(m_fallback_codepage.first));
		AR_WIN_VERIFY(SetConsoleCP(m_fallback_codepage.second));
	}
	
	TermVert WinTerminalRenderer::termSize() const
	{
		// TODO: this might work with a different setup?
		// return ARApp::getApplication()->getTermEvtHandler().getEvtListener().winLastTermSize();
		
		// calculate the cols / rows using the size and fontsize of the console
		
		RECT client_rect;

		AR_WIN_VERIFY(GetClientRect(m_console_hwin, &client_rect));
		
		TermVert size(client_rect.right - client_rect.left, client_rect.bottom - client_rect.top);

		size = size.cwiseQuotient((TermVert)getFont().second).eval();

		return size;
	}

	TermVert WinTerminalRenderer::maxSize() const
	{
		COORD max_size = GetLargestConsoleWindowSize(m_buffer.getCBuffers()[0]);

		return { max_size.X, max_size.Y };
	}
	
	TermVert WinTerminalRenderer::pos() const
	{
		RECT pos;
		AR_WIN_VERIFY(GetWindowRect(m_console_hwin, &pos));

		long long x = pos.left;
		long long y = pos.top;

		return { (Real)x, (Real)y };
	}

	void WinTerminalRenderer::resizeBuff()
	{
		Size2D resize_size = drawSize().unaryExpr([](size_t val) { return val + 1; }).cwiseProduct(getFont().second);

		// resizing with terminal escape sequnces on windows is very unreliable and will cause the terminal to crash more than often
		// so here SetWIndowPos is used instead as an alternative.
		CT_MEASURE_N("BUFFER RESIZE");
		// calculate the width in pixels using the font size, and requested buffer size.
		RECT window_rect;
		RECT client_rect;

		AR_WIN_VERIFY(GetClientRect(m_console_hwin, &client_rect));

		// convert client coords to display coords

		AR_WIN_VERIFY(ClientToScreen(m_console_hwin, (POINT*)&client_rect));
		AR_WIN_VERIFY(ClientToScreen(m_console_hwin, ((POINT*)&client_rect) + 1));

		AR_WIN_VERIFY(GetWindowRect(m_console_hwin, &window_rect));

		// -2 = account for the 1 pixel wide blue border
		LONG border_height = client_rect.top - window_rect.top + window_rect.bottom - client_rect.bottom - 2;
		LONG border_width = client_rect.left - window_rect.left + window_rect.right - client_rect.right - 2;

		AR_WIN_VERIFY(SetWindowPos(m_console_hwin, NULL, -1, -1, (int)resize_size.x + border_width, (int)resize_size.y + border_height, SWP_NOMOVE | SWP_NOZORDER));
	}

	std::pair<std::string, Size2D> WinTerminalRenderer::getFont() const
	{
		CONSOLE_FONT_INFOEX font_info{sizeof(CONSOLE_FONT_INFOEX)};

		std::pair<std::string, Size2D> result;

		// both buffers should have the same font
		AR_WIN_VERIFY(GetCurrentConsoleFontEx(m_buffer.getCBuffers()[0], FALSE, &font_info));

		std::wstring_view name = font_info.FaceName;

	#pragma warning(push)
	#pragma warning(suppress: 4244)
		result.first = std::string(name.begin(), name.end());
		result.second = { font_info.dwFontSize.X, font_info.dwFontSize.Y };
	#pragma warning(pop)

		return result;
	}

	bool WinTerminalRenderer::setFont(const std::string& name, Size2D size)
	{
		auto font = getFont();

		// only change terminal font if it is different from the current font
		if (font.first != name || font.second != size)
		{
			AR_ASSERT_MSG(name.size() < LF_FACESIZE, "Font name cannot be more than 32 characters long");
			CONSOLE_FONT_INFOEX font_info{ sizeof(CONSOLE_FONT_INFOEX) };

			// default font weight is 400
			font_info.FontWeight = 400;

			font_info.dwFontSize = { (SHORT)size.x, (SHORT)size.y };

			for (size_t i = 0; i < name.size(); i++)
				font_info.FaceName[i] = name[i];


			bool success = true;

			// font should be the same for full screen and windowed
			for (HANDLE hconsole : m_buffer.getCBuffers())
			{
				bool max_font_res = SetCurrentConsoleFontEx(hconsole, TRUE, &font_info);
				AR_WIN_VERIFY(max_font_res);
				bool win_font_res = SetCurrentConsoleFontEx(hconsole, FALSE, &font_info);
				AR_WIN_VERIFY(win_font_res);

				success = success && max_font_res && win_font_res;
			}

			return success;
		}

		return true;
	}

	bool WinTerminalRenderer::isFocused() const
	{
		return GetForegroundWindow() == GetConsoleWindow();
	}

}
}
