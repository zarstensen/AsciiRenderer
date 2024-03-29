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
		replaceCBuffers();

		DWORD mode;
		GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &mode);

		AR_WIN_VERIFY(SetConsoleMode(m_hconsole_writable, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING));
		AR_WIN_VERIFY(SetConsoleMode(m_hconsole_display, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING));

		AR_WIN_VERIFY(SetConsoleActiveScreenBuffer(m_hconsole_display));
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

	// this should be used sparingly in the windows implementation, as it writes to both the writable and display buffer in order to sync the data.
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

	void TermRendererBuffer::replaceCBuffers()
	{
		HANDLE new_display = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		HANDLE new_writable = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

		CONSOLE_FONT_INFOEX font_info{ sizeof(CONSOLE_FONT_INFOEX) };

		// both buffers should have the same font
		AR_WIN_VERIFY(GetCurrentConsoleFontEx(m_hconsole_display, FALSE, &font_info));

		AR_WIN_VERIFY(SetConsoleActiveScreenBuffer(new_display));

		// only close the handle if it is valid
		if (m_hconsole_display && m_hconsole_display != INVALID_HANDLE_VALUE)
			CloseHandle(m_hconsole_display);
		if (m_hconsole_writable && m_hconsole_writable != INVALID_HANDLE_VALUE)
			CloseHandle(m_hconsole_writable);

		m_hconsole_display = new_display;
		m_hconsole_writable = new_writable;

		AR_WIN_VERIFY(SetCurrentConsoleFontEx(m_hconsole_display, FALSE, &font_info));
		AR_WIN_VERIFY(SetCurrentConsoleFontEx(m_hconsole_display, TRUE, &font_info));
		AR_WIN_VERIFY(SetCurrentConsoleFontEx(m_hconsole_writable, FALSE, &font_info));
		AR_WIN_VERIFY(SetCurrentConsoleFontEx(m_hconsole_writable, TRUE, &font_info));
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

		m_font_size = getFont().second;

		// prevent the terminal from being fullscreenable and resizable,
		// in order to reduce the frequency of crashes caused by this bug [https://developercommunity.visualstudio.com/t/Visual-studio-debugger-console-crashes-i/10107748].
		LONG fallback_style = GetWindowLong(m_console_hwin, GWL_STYLE);
		SetWindowLong(m_console_hwin, GWL_STYLE, fallback_style & ~(WS_SIZEBOX | WS_MAXIMIZEBOX));

		initRenderer(props);
	}

	WinTerminalRenderer::~WinTerminalRenderer()
	{

		// reset console mode
		//AR_WIN_VERIFY(SetConsoleMode(m_console, m_fallback_console_mode));
		AR_WIN_VERIFY(SetConsoleOutputCP(m_fallback_codepage.first));
		AR_WIN_VERIFY(SetConsoleCP(m_fallback_codepage.second));
	}

	// simply use the builtin system metric to get the count
	// 0 = failed
	size_t WinTerminalRenderer::monitorCount() const
	{
		int monitor_count = GetSystemMetrics(SM_CMONITORS);
		AR_WIN_VERIFY(monitor_count);
		return monitor_count;
	}

	TermVert WinTerminalRenderer::monitorSize(size_t display) const
	{
		// TODO: implement this
		assert(false);
		return TermVert();
	}

	TermVert WinTerminalRenderer::monitorSize() const
	{
		// get the monitor handle the console window is currently on.
		HMONITOR monitor_handle = MonitorFromWindow(m_console_hwin, MONITOR_DEFAULTTOPRIMARY);

		AR_WIN_VERIFY(monitor_handle);

		// retrieve the monitor rectangle and return the width and height of it.

		MONITORINFO info{ sizeof(MONITORINFO) };

		AR_WIN_VERIFY(GetMonitorInfo(monitor_handle, &info));

		return { info.rcMonitor.right - info.rcMonitor.left, info.rcMonitor.bottom - info.rcMonitor.top };
	}

	TermVert WinTerminalRenderer::workSize() const
	{
		RECT work_rect;
		SystemParametersInfo(SPI_GETWORKAREA, NULL, &work_rect, NULL);

		return TermVert();
	}
	
	TermVert WinTerminalRenderer::termSize() const
	{
		// TODO: this might work with a different setup?
		// return ARApp::getApplication()->getTermEvtHandler().getEvtListener().winLastTermSize();
		
		// calculate the cols / rows using the size and fontsize of the console
		
		CONSOLE_SCREEN_BUFFER_INFO console_info;

		AR_WIN_VERIFY(GetConsoleScreenBufferInfo(m_buffer.getCBuffers()[0], &console_info));
		
		TermVert size(console_info.dwSize.X, console_info.dwSize.Y);

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

	// resize visible buffer, create two new buffers to replace the visible and drawable.
	// this makes sure the screen buffer size matches the terminal dimensions.
	// finally, the two new buffers replace the old ones, which in turn are closed.
	bool WinTerminalRenderer::resizeBuff()
	{
		CT_MEASURE_N("BUFFER RESIZE");
		TermVert resize_size = (TermVert)drawSize();

		// in case the maximum size has changed since the last resize, cap the resize_size to be inside the maximum bounds, even though this check has already happened.
		resize_size.x = std::min(maxSize().x, resize_size.x);
		resize_size.y = std::min(maxSize().y, resize_size.y);

		// set the size of the visible buffer
		
		// in order to minimize heap allocation during string operations, the dimensions of a resize is assumed to always be less than 9999 (less than 5 digits in length).
		// due to Windows being wierd, we must first resize to 1 x height and then width x height,
		// otherwise the terminal will not resize properly in some cases.

		std::string resize_str = AR_ANSI_CSI "8;1;XXXXt" AR_ANSI_CSI "8;XXXX;XXXXt";
		constexpr size_t digit_count = 4;

		// the resize escape sequence accepts values with leading zeroes, so this is how the values are inserted into the resize string.
		
		auto resize_iter = resize_str.begin();
		char num_buff[digit_count + 1];

		// first height
		resize_iter += strlen(AR_ANSI_CSI) + strlen("8;1;");
		std::snprintf(num_buff, digit_count + 1, "%04d", resize_size.x);
		resize_iter = std::copy(num_buff, num_buff + digit_count, resize_iter);

		// first width
		resize_iter += strlen("t") + strlen(AR_ANSI_CSI) + strlen("8;");
		std::snprintf(num_buff, digit_count + 1, "%04d", resize_size.y);
		resize_iter = std::copy(num_buff, num_buff + digit_count, resize_iter);


		// second height
		resize_iter += strlen(";");
		std::snprintf(num_buff, digit_count + 1, "%04d", resize_size.x);
		std::copy(num_buff, num_buff + digit_count, resize_iter);


		AR_WIN_VERIFY(WriteFile(m_buffer.getCBuffers()[0], resize_str.data(), resize_str.size(), NULL, NULL));

		m_buffer.replaceCBuffers();

	#if 0 // TODO: remove this when above code has been tested
		TermVert term_size = termSize();
		// sometimes the console will have a larger window size than the maximum window size. As this creates problems for the code below, the window size is clamped to its maximum possible size.
		// this situation might occur if the console window size is changed with a "zoom", which does not update the window size, even if the window size grows.
		if (term_size.x > maxSize().x || term_size.y > maxSize().y)
		{
			SMALL_RECT max_win_size = { 0, 0, (SHORT)std::min(maxSize().x, term_size.x) - 1, (SHORT)std::min(maxSize().y, term_size.y) - 1 };
			AR_WIN_VERIFY(SetConsoleWindowInfo(m_buffer.getCBuffers()[0], TRUE, &max_win_size));
		}

		term_size.x = std::min(maxSize().x, term_size.x);
		term_size.y = std::min(maxSize().y, term_size.y);

	#if 0 // this is now done in the windows initializing function.
		// prevent resizing momentarily or else the resize might happen inbetween SetConsoleWindowInfo and SetConsoleScreenBufferSize, which will trigger an windows error.

		LONG fallback_style = GetWindowLong(m_console_hwin, GWL_STYLE);
		SetWindowLong(m_console_hwin, GWL_STYLE, fallback_style & ~(WS_SIZEBOX));
	#endif

		// console window size cannot be greater than the console buffer size, and the console buffer size cannot be smaller than the console window size,
		// So depending on the current size, and the new size, the order of the functions should be flipped.
		// this is true independent of the direction, so if the new width is smaller than the old width, but the new height is larger than the old height, the resize operation must be split.

		COORD buff_width{ resize_size.x, term_size.y};
		SMALL_RECT win_width{ 0, 0, buff_width.X - 1, buff_width.Y - 1 };

		// TODO: check if the code below this is required, alternative code: ignore if side nr. 1 needs to be downscaled and side nr. 2 needs to be upscaled (less win32 calls).
		if (term_size.x > resize_size.x)
		{
			AR_WIN_CHECK(SetConsoleWindowInfo(m_buffer.getCBuffers()[0], TRUE, &win_width), success);
			AR_WIN_CHECK(SetConsoleScreenBufferSize(m_buffer.getCBuffers()[0], buff_width), success);
		}
		else if (term_size.x < resize_size.x)
		{
			AR_WIN_CHECK((SetConsoleScreenBufferSize(m_buffer.getCBuffers()[0], buff_width)), success);
			AR_WIN_CHECK(SetConsoleWindowInfo(m_buffer.getCBuffers()[0], TRUE, &win_width), success);
		}
		
		COORD buff_height{ resize_size.x, resize_size.y };
		SMALL_RECT win_height{ 0, 0, buff_height.X - 1, buff_height.Y - 1 };

		if(term_size.y > resize_size.y)
		{
			AR_WIN_CHECK(SetConsoleWindowInfo(m_buffer.getCBuffers()[0], TRUE, &win_height), success);
			AR_WIN_CHECK(SetConsoleScreenBufferSize(m_buffer.getCBuffers()[0], buff_height), success);
		}
		else if (term_size.y < resize_size.y)
		{
			AR_WIN_CHECK(SetConsoleScreenBufferSize(m_buffer.getCBuffers()[0], buff_height), success);
			AR_WIN_CHECK(SetConsoleWindowInfo(m_buffer.getCBuffers()[0], TRUE, &win_height), success);
		}

		// window resize failed (happens randomly sometimes)
		// TODO: investigate this
		if (!success)
		{
			// TODO: do something here?
		}

	#if 0
		SetWindowLong(m_console_hwin, GWL_STYLE, fallback_style);
	#endif

		return success;
	#endif

		return true;
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

			m_font_size = size;

			return success;
		}

		return true;
	}

	bool WinTerminalRenderer::isFocused() const
	{
		return GetForegroundWindow() == GetConsoleWindow();
	}

	void WinTerminalRenderer::flushBuffer()
	{
		CT_MEASURE_N("BUFFER FLUSH");

		{
			CT_MEASURE_N("BUFFER SWAP");
			m_buffer.swapCBuffer();
		}

		{
			CT_MEASURE_N("FONT SYNC");
			// the two buffers should not have different fonts, so make sure they are the same here.
			if (m_font_size != getFont().second)
				setFont(getFont().first, m_font_size);
		}
	}
}
}
