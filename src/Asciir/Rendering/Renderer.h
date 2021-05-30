#pragma once

#include "TerminalRenderer.h"
#include "Asciir/Math/Vertices.h"

namespace Asciir
{
	class Renderer
	{
	public:

		static void init();

		// graphic functions
		static void drawPolygon(Coords verts, Tile tile);

		static void drawRect(s_Coords<2> verts, Tile tile);

		// terminal functions

		static void resize(Size2D size);
		static Size2D size();

	protected:

		static void projectCoordsToTerminal(Coords& coords);

		template<size_t n>
		static void projectCoordsToTerminal(s_Coords<n>& coords);

		static TerminalRenderer* s_renderer;
		static const AsciiAttr* s_attr_handler;
	};
}
