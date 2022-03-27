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

		/// @brief constructs a quad from the given top left and bottom right coordinate
		static Quad fromCorners(const Coord& top_left, const Coord& bottom_right) { return Quad(bottom_right - top_left, top_left); }

		/// @brief retrieve the center point of the quad, with the passed transform
		Coord centrePoint(Transform& transform) const;
		Coord centrePoint() { Transform t; return centrePoint(t); }

		/// @brief checks if a coordinate is inside the quad with the passed transform applied
		bool isInside(Coord coord, Transform& transform) const;
		bool isInside(Coord coord) { Transform t; return isInside(coord, t); }

		/// @brief checks if a coordinate is inside the quad, fitted within a grid with the given resolution, and with the passed transform applied.
		bool isInsideGrid(Coord coord, Transform& transform, Real resolution = 1) const;
		bool isInsideGrid(Coord coord, Real resolution = 1) const { Transform t; return isInsideGrid(coord, t, resolution); }

		/// @brief get the 4 vertices constructing the quad represented
		s_arVertices<Real, 2, 4> getVerts()
		{
			s_arVertices<Real, 2, 4> verts;
			verts << offset, Coord(offset.x, size.y), size, Coord(size.x, offset.y);
			return verts;
		}

		bool operator==(const Quad& other)
		{
			return offset == other.offset && size == other.size;
		}

	};

	/// @brief outputs the offset and the quad size to the passed stream
	inline std::ostream& operator<<(std::ostream& stream, const Quad& quad)
	{
		stream << quad.offset << ' ' << quad.size;
		return stream;
	}

}
