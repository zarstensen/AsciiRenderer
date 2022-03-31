#pragma once

#include "Asciir/Rendering/TerminalRenderer.h"

namespace Asciir
{
namespace TRInterface
{
    class MacTerminalRenderer: public TerminalRendererInterface
    {
	public:
        MacTerminalRenderer(const TerminalRendererInterface::TerminalProps& props)
            : TerminalRendererInterface(props) {}

		/// @see TerminalRendererInterface::termSize()
		TermVert termSize() const;
		/// @see TerminalRendererInterface::maxSize()
		TermVert maxSize() const { return {500, 500}; }
		/// @see TerminalRendererInterface::pos()
		Coord pos() const { return {0, 0}; }
		/// @see TerminalRendererInterface::fontSize()
		Size2D fontSize() const { return {1, 1}; }

		/// @see TerminalRendererInterface::isFocused()
		bool isFocused() const { return true; }
    };
}

typedef TRInterface::MacTerminalRenderer TerminalRenderer;

}