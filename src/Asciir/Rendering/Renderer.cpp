#include "pch/arpch.h"
#include "Renderer.h"
#include "Asciir/Core/Engine.h"
#include "Asciir/Math/Lines.h"
#include "Asciir/Math/Vertices.h"

namespace Asciir
{

	TerminalRenderer* Renderer::s_renderer = nullptr;
	const AsciiAttr* Renderer::s_attr_handler = nullptr;

	void Renderer::init()
	{
		s_renderer = &AREngine::getEngine()->getTerminal().getRenderer();
		s_attr_handler = &s_renderer->getAttrHandler();
	}

	void Renderer::drawMesh(const Mesh& mesh, Tile tile)
	{
		Tile prev_state = s_renderer->getState();
		s_renderer->setState(tile);
		
		Coord top_left_coord(size());
		Coord bottom_right_coord(0, 0);

		for (const Coord& vert : mesh.getVerts())
		{
			top_left_coord.x = std::min(top_left_coord.x, vert.x);
			top_left_coord.y = std::min(top_left_coord.y, vert.y);

			top_left_coord.x = top_left_coord.x < 0 ? 0 : top_left_coord.x;
			top_left_coord.y = top_left_coord.y < 0 ? 0 : top_left_coord.y;

			bottom_right_coord.x = std::max(bottom_right_coord.x, vert.x);
			bottom_right_coord.y = std::max(bottom_right_coord.y, vert.y);

			bottom_right_coord.x = bottom_right_coord.x >= size().x ? size().x - 1 : bottom_right_coord.x;
			bottom_right_coord.y = bottom_right_coord.y >= size().y ? size().y - 1 : bottom_right_coord.y;
		}

		for (TInt y = top_left_coord.y; y < bottom_right_coord.y + 1; y++)
			for (TInt x = top_left_coord.x; x < bottom_right_coord.x + 1; x++)
				if (mesh.isInside({ x, y })) s_renderer->drawTile({x, y});

		s_renderer->setState(prev_state);
	}

	void Renderer::drawRect(s_Coords<2> verts, Tile tile)
	{
		Mesh rect({ verts[0], {verts[1].x, verts[0].y }, verts[1], {verts[0].x, verts[1].x} });

		drawMesh(rect, tile);
	}

	void Renderer::drawTile(Coord pos, Tile tile)
	{
		if (pos.x >= 0 && (size_t )pos.x < size().x && pos.y >= 0 && (size_t) pos.y < size().y)
		{
			Tile prev_state = s_renderer->getState();
			s_renderer->setState(tile);
			s_renderer->drawTile((TermVert) pos);
			s_renderer->setState(prev_state);
		}
	}

	Tile Renderer::viewTile(Coord pos)
	{
		AR_ASSERT_MSG(pos.x >= 0 && (size_t) pos.x < size().x && pos.y >= 0 && (size_t) pos.y < size().y, "Cannot view tile outside of terminal size");
		return s_renderer->getTile((TermVert)pos);
	}

	void Renderer::clear(Tile tile)
	{
		s_renderer->clearTerminal(tile);
	}

	void Renderer::resize(Size2D size)
	{
		s_renderer->resize(size);
	}
	
	Size2D Renderer::size()
	{
		return s_renderer->drawSize();
	}
}
