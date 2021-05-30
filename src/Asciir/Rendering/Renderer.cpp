#include "pch/arpch.h"
#include "Renderer.h"
#include "Asciir/Core/Engine.h"


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
		projectCoordsToTerminal(verts);

		s_renderer->setState(tile);
		s_renderer->drawVertices(verts, DrawMode::Filled);
	}

	void Renderer::drawRect(s_Coords<2> verts, Tile tile)
	{
		s_renderer->setState(tile);

		projectCoordsToTerminal(verts);

		Coord tl = verts[0];
		Coord br = verts[1];
		Coord tr = { br.x, tl.y };
		Coord bl = { tl.x, br.y };

		s_renderer->drawVertices({ tr, tl, bl, br }, DrawMode::Filled);
	}
	
	// performs min & max functions on the coords so it does not go outside terminal bounds
	void Renderer::projectCoordsToTerminal(Coords& coords)
	{

		Size2D term_size = size();

		for (auto& elem : coords)
		{
			Coord& coord = (Coord&)elem;

			coord.x = std::min(std::max(coord.x, 0LL), (long long) term_size.x - 1);
			coord.y = std::min(std::max(coord.y, 0LL), (long long) term_size.y - 1);
		}
	}

	template<size_t n>
	void Renderer::projectCoordsToTerminal(s_Coords<n>& coords)
	{

		Size2D term_size = size();

		for (auto& elem : coords)
		{
			try
			{
				Coord& coord = elem;

				coord.x = std::min(std::max(coord.x, 0LL), (long long)term_size.x - 1);
				coord.y = std::min(std::max(coord.y, 0LL), (long long)term_size.y - 1);
			}
			catch (std::exception& e)
			{
				std::cout << e.what();
				throw e;
			}
		}
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
