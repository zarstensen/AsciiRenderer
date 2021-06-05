#include "Renderer.h"
#include "Asciir/Math/Lines.h"

namespace Asciir
{
    template<typename T, std::enable_if_t<is_vertices_type_v<T>, bool>>
	Coords Renderer::projectCoordsToTerminal(const T& coords)
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

		for (size_t i = 0; i < (size_t) coords.size(); i++)
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


		for (size_t i = 0; i < (size_t) coords.size(); i++)
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
}