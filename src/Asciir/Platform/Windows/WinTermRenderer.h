#pragma once

#include "Asciir/Rendering/TerminalRenderer.h"

namespace Asciir
{
namespace TRInterface
{
	//
	// the windows implementation of the TermRendererBuffer uses two seperate console buffers, a writable, and a display buffer.
	// the reason for this is that it is much faster to write to a console buffer that is not currently being displayed,
	// thus a buffer swap system is implemented with the console buffers, where each console print, will result in the buffers being swapped.
	//
	class TermRendererBuffer : public std::streambuf
	{
	public:
		/// @brief constructs a TermRendererBuffer instance
		/// @param buffer_size the size of the buffer
		TermRendererBuffer(size_t buffer_size)
		{
			m_buffer.reserve(buffer_size);

			// allocate console buffers
			m_hconsole_display = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
			m_hconsole_writable = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

			AR_WIN_VERIFY(SetConsoleActiveScreenBuffer(m_hconsole_display));

		}

		~TermRendererBuffer()
		{
			// reset console buffer and close the previously allocated ones.
			AR_WIN_VERIFY(SetConsoleActiveScreenBuffer(NULL));

			AR_WIN_VERIFY(CloseHandle(m_hconsole_display));
			AR_WIN_VERIFY(CloseHandle(m_hconsole_writable));
		}

		/// @brief put *s* into the buffer
		/// @param s string data
		/// @param count length of string data
		/// @return count
		std::streamsize xsputn(const std::streambuf::char_type* s, std::streamsize count) final override;

		/// @brief put single char into the buffer
		/// @return state of the overflow operation
		std::streambuf::int_type overflow(std::streambuf::int_type ch) final override;

		/// @brief flushes the buffer into stdout.
		int sync() final override;

		/// @brief swaps the console buffers, and makes sure the previously displayed buffer, is up to date with the writable buffer.
		/// @note this also syncs the buffer.
		void swapCBuffer();

		/// @brief returns an array containing the two console buffers
		/// @return first = display, second = writable
		std::array<HANDLE, 2> getCBuffers() const { return { m_hconsole_display, m_hconsole_writable }; }

	private:
		std::string m_buffer;

		// handle to the currently displayed console buffer
		HANDLE m_hconsole_display;
		// handle to the currently writable console buffer
		HANDLE m_hconsole_writable;
	};

	namespace ELInterface { class WinEventListener; }

	/// @brief Windows specific implementation of TerminalRenderer.   
	/// makes use of the windows api in order to retrieve information about the terminal size and position
	class WinTerminalRenderer: public TerminalRendererInterface
	{
	public:

		friend ELInterface::WinEventListener;

		/// @brief sets up the windows console to accept ansi codes
		WinTerminalRenderer(const WinTerminalRenderer::TerminalProps& props);
		/// @brief reverts the console to before WinTerminalRenderer was instansiated
		~WinTerminalRenderer();

		/// @see TerminalRendererInterface::termSize()
		TermVert termSize() const;
		/// @see TerminalRendererInterface::maxSize()
		TermVert maxSize() const;
		/// @see TerminalRendererInterface::pos()
		Coord pos() const;
		
		/// @see TerminalRendererInterface::getFont()
		std::pair<std::string, Size2D> getFont() const;

		/// @see TerminalRendererInterface::setFont()
		bool setFont(const std::string& name, Size2D size);

		/// @see TerminalRendererInterface::isFocused()
		bool isFocused() const;

		/// @see TerminalRendererInterface::flushBuffer()
		void flushBuffer() { m_buffer.swapCBuffer(); }

		/// @see TerminalRendererInterface::getBuffer()
		/// @return 
		std::streambuf* getBuffer() { return &m_buffer; }

	protected:

		// handle to the current console
		HANDLE m_console;
		// handle to the current console window
		HWND m_console_hwin;
		// console mode before constructor
		DWORD m_fallback_console_mode;
		// codepage before constructor
		std::pair<UINT, UINT> m_fallback_codepage;
		// the current font size, defaults to the console font size
		COORD m_font_size;

		TermRendererBuffer m_buffer;
	};
}

typedef TRInterface::WinTerminalRenderer TerminalRenderer;

}