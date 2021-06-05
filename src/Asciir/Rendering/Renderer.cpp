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
		constexpr bool val = is_vertices_type_v<s_Coords<2>>;
		Coords projected_verts = projectCoordsToTerminal<Coords>(verts);

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

		projectCoordsToTerminal(verts);

		Coord tl = verts[0];
		Coord br = verts[1];
		Coord tr = { br.x, tl.y };
		Coord bl = { tl.x, br.y };

		s_renderer->drawVertices({ tr, tl, bl, br }, DrawMode::Filled);
		s_renderer->setState(prev_state);
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
