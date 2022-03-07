#pragma once

#include "Asciir/Rendering/TerminalRenderer.h"

namespace Asciir
{
namespace TRInterface
{
	/// @brief Windows specific implementation of TerminalRenderer.   
	/// makes use of the windows api in order to retrieve information about the terminal size and position
	class WinTerminalRenderer: public TerminalRendererInterface
	{
	public:
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
		/// @see TerminalRendererInterface::fontSize()
		Size2D fontSize() const;

	protected:

		// handle to the current console
		HANDLE m_console;
		// console mode before constructor
		DWORD m_fallback_console_mode;
		// codepage before constructor
		UINT m_fallback_codepage;
	};
}

typedef TRInterface::WinTerminalRenderer TerminalRenderer;

}