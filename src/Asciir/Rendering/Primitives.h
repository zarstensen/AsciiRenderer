#pragma once

#include "Asciir/Maths/Vertices.h"
#include "Mesh.h"

namespace Asciir
{
	/// @brief class representing a quad, as well as implementing less expensive functions from the Mesh class.  
	///
	/// the quad is stored as the position of the top right corner of the quad, and the size of the quad.
	/// 
	class Quad
	{

	public:

		Coord offset;
		Coord size;
		
		Quad() = default;

		Quad(Coord size, Coord offset = Coord(0, 0))
			: offset(offset), size(size) {}


		/// @brief retrieve the center point of the quad, with the passed transform
		Coord centrePoint(const Transform& transform = NoTransform) const;

		/// @brief checks if a coordinate is inside the quad with the passed transform applied
		bool isInside(Coord coord, const Transform& transform = NoTransform) const;
		/// @brief checks if a coordinate is inside the quad, fitted within a grid with the given resolution, and with the passed transform applied.
		bool isInsideGrid(const Coord& boord, Real resolution, const Transform& transform = NoTransform) const;

		/// @brief get the 4 vertices constructing the quad represented
		s_arVertices<Real, 2, 4> getVerts()
		{
			s_arVertices<Real, 2, 4> verts;
			verts << offset, Coord(offset.x, size.y), size, Coord(size.x, offset.y);
			return verts;
		}

	};

	/// @brief outputs the offset and the quad size to the passed stream
	inline std::ostream& operator<<(std::ostream& stream, const Quad& quad)
	{
		stream << quad.offset << ' ' << quad.size;
		return stream;
	}

}
