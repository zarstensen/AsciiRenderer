#include "pch/arpch.h"
#include "Renderer.h"
#include "Asciir/Core/Engine.h"
#include "Asciir/Math/Lines.h"

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
		AR_CORE_INFO(verts);
		Coords projected_verts = projectCoordsToTerminal(verts);
		AR_CORE_INFO(projected_verts);

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
	
	Coords Renderer::projectCoordsToTerminal(const Coords& coords)
	{
		RealVertex term_size = size();
		
		std::vector<Line> terminal_box = {  
			Line::fromPoints({ term_size.x,	term_size.y }, { 0, term_size.y }), // top line		<-------↑
			Line::fromPoints({ 0, term_size.y }, { 0, 0 }),						// left line	|		|
			Line::fromPoints({ 0, 0 }, { term_size.x, 0 }),						// bottom line	|		|
			Line::fromPoints({ term_size.x,	0 }, { term_size.x, term_size.y })	// right line	↓------->
		};

		// get number of verticies in new mesh

		size_t res_size = 0;
		Coord last_coord = coords[coords.size() - 1];

		for (size_t i = 0; i < coords.size(); i++)
		{
			const Coord& coord = coords[i];

			if (Line::visibleByAll(terminal_box, coord))
				res_size++;
			

			if (Line::visibleByAll(terminal_box, coord) != Line::visibleByAll(terminal_box, last_coord))
				res_size++;

			last_coord = coord;
		}

		Coords result(res_size);
		size_t res_indx = 0;


		for (size_t i = 0; i < coords.size(); i++)
		{
			const Coord& coord = coords[i];
			
			if (Line::visibleByAll(terminal_box, last_coord))
			{
				result[res_indx] = last_coord;
				res_indx++;
			}
			
			for (size_t i = 0; i < terminal_box.size(); i++)
			{
				if (terminal_box[i].visible(coord) != terminal_box[i].visible(last_coord))
				{
					result[res_indx] = terminal_box[i].intersect(Line::fromPoints(coord, last_coord));
					res_indx++;
					break;
				}
			}

			last_coord = coord;
		}

		return result;
	}

	template<size_t n>
	Coords Renderer::projectCoordsToTerminal(const s_Coords<n>& coords)
	{

		Size2D term_size = size();

		for (const Coord& coord : coords)
		{
			try
			{
				coord.x = std::min(std::max(coord.x, 0LL), (long long)term_size.x - 1);
				coord.y = std::min(std::max(coord.y, 0LL), (long long)term_size.y - 1);
			}
			catch (std::exception& e)
			{
				std::cout << e.what();
				return {};
				throw e;
			}
		}
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
