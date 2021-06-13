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

	void Renderer::drawPolygon(Coords verts, Tile tile)
	{
		Coords projected_verts = projectCoordsToTerminal(verts);

		AR_INFO(projected_verts);

		Tile prev_state = s_renderer->getState();
		s_renderer->setState(tile);
		s_renderer->drawVertices(projected_verts, DrawMode::Filled);
		s_renderer->setState(prev_state);
	}

	void Renderer::drawRect(s_Coords<2> verts, Tile tile)
	{
		Tile prev_state = s_renderer->getState();
		s_renderer->setState(tile);

		Coords projected_verts = projectCoordsToTerminal(verts);

		Coord tl = projected_verts[0];
		Coord br = projected_verts[1];
		Coord tr = { br.x, tl.y };
		Coord bl = { tl.x, br.y };

		s_renderer->drawVertices({ tr, tl, bl, br }, DrawMode::Filled);
		s_renderer->setState(prev_state);
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
