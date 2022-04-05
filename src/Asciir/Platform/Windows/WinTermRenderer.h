#pragma once

#include "Asciir/Rendering/TerminalRenderer.h"

namespace Asciir
{
namespace TRInterface
{
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
	};
}

typedef TRInterface::WinTerminalRenderer TerminalRenderer;

}