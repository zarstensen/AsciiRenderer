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
		static void drawTile(Coord pos, Tile tile);

		// terminal functions

		static void clear(Tile tile = Tile(BLACK8, WHITE8, ' '));
		static void resize(Size2D size);
		static Size2D size();

	protected:

		template<typename T, std::enable_if_t<is_vertices_type_v<T>, bool> = false>
		static Coords projectCoordsToTerminal(const T& coords);

		static TerminalRenderer* s_renderer;
		static const AsciiAttr* s_attr_handler;
	};
}

#include "Renderer.ipp"
