#pragma once

#include "Asciir/Rendering/TerminalRenderer.h"

namespace Asciir
{
namespace TRInterface
{
    class MacTerminalRenderer: public TerminalRendererInterface
    {
        MacTerminalRenderer(const WinTerminalRenderer::TerminalProps& props)
            : TerminalRendererInterface(props) {}

		/// @see TerminalRendererInterface::termSize()
		TermVert termSize() const;
		/// @see TerminalRendererInterface::maxSize()
		TermVert maxSize() const;
		/// @see TerminalRendererInterface::pos()
		Coord pos() const;
		/// @see TerminalRendererInterface::fontSize()
		Size2D fontSize() const;

		/// @see TerminalRendererInterface::isFocused()
		bool isFocused() const;
    }
}
}