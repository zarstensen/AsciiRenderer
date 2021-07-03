#include "pch/arpch.h"
#include "Mesh.h"
#include "Asciir/Math/Matrix.h"
#include "Asciir/Math/Math.h"

namespace Asciir
{

	Coord Transform::applyTransform(const Coord& vec) const
	{
		// uses Eigen transform matrices to perform the transformation

		auto origin_transform = Eigen::Translation<Real, 2>(origin).inverse() * vec;

		auto scale_transform = Eigen::DiagonalMatrix<Real, 2>(scale) * origin_transform;

		auto rotation_transform = Eigen::Rotation2D<Real>(rotation) * scale_transform;

		auto move_transform = Eigen::Translation<Real, 2>(pos) * rotation_transform;

		auto result = Eigen::Translation<Real, 2>(origin) * move_transform;

		return result;
	}

	Coord Transform::reverseTransform(const Coord& vec) const
	{
		// uses Eigen transform matrices to perform the transformation

		auto origin_transform = Eigen::Translation<Real, 2>(origin).inverse() * vec;

		auto move_transform = Eigen::Translation<Real, 2>(pos).inverse() * origin_transform;

		auto rotation_transform = Eigen::Rotation2D<Real>(rotation).inverse() * move_transform;

		auto scale_transform = Eigen::DiagonalMatrix<Real, 2>(scale).inverse() * rotation_transform;

		auto result = Eigen::Translation<Real, 2>(origin) * rotation_transform;

		return result;
	}

	Mesh::Mesh(const Coords& vertices)
		: m_vertices(vertices), m_faces(m_vertices.size() + 1), m_face_count(1)
	{
		for (size_t i = 0; i < m_faces.size(); i++)
			m_faces[i] = i;

		m_faces[m_faces.size() - 1] = 0;
	}

	Mesh::Mesh(const std::vector<Coords>& polygon)
	{
		size_t vert_count = 0;
		size_t edge_count = 0;

		for (const Coords& verts : polygon)
		{
			vert_count += verts.size();
			edge_count += verts.size() + 1;
		}
			

		m_vertices.resize(vert_count);
		m_faces.reserve(vert_count);

		size_t face_indx = 0;
		size_t index_offset = 0;

		addFace(index_offset);
		
		m_vertices[index_offset] = polygon[0][0];
		for (size_t j = 1; j < polygon[0].size(); j++)
		{
			m_vertices[index_offset + j] = polygon[0][j];
			extendFace(0, index_offset + j);
		}

		index_offset += polygon[0].size();

		for (size_t i = 1; i < polygon.size(); i++)
		{
			addFace(index_offset + polygon[i].size() - 1);
			m_vertices[index_offset] = polygon[i][0];
			for (size_t j = 1; j < polygon[i].size(); j++)
			{
				m_vertices[index_offset + j] = polygon[i][j];
				extendFace(i, index_offset + polygon[i].size() - j - 1);
			}

			index_offset += polygon[i].size();
		}
	}

	Mesh::Mesh(const Coords& vertices, const std::vector<size_t>& faces)
		: m_vertices(vertices), m_faces(faces)
	{
		if (m_faces.size() > 0)
		{
			size_t face_start = m_faces[0];

			for (size_t i = 1; i < m_faces.size() - 1; i++)
				if (face_start == m_faces[i])
				{
					i++;
					face_start = m_faces[i];
					m_face_count++;
				}

			AR_ASSERT_MSG(face_start == m_faces[m_faces.size() - 1], "Missing end of face");

			m_face_count++;
		}
		else
			m_face_count = 0;

		#ifdef AR_DEBUG
		for(size_t i = 0; i < m_face_count; i++)
			for (size_t j = 0; j < faceCornerCount(i); j++)
			{
				AR_ASSERT_MSG(std::count(m_faces.begin() + firstIndexFromFace(i), m_faces.begin() + lastIndexFromFace(i), getCorner(i, j)) == 1,
					"Cannot have more than one vertex in a face, unless it is the first vertex");
				AR_ASSERT_MSG(m_faces[firstIndexFromFace(i) + j] < m_vertices.size(), "Face corner is out of bounds: ", m_faces[firstIndexFromFace(i) + j]);
			}
		#endif
	}
	
	LineSegment Mesh::getEdge(size_t face_index, size_t index, const Transform& transform) const
	{
		AR_ASSERT_MSG(face_index < m_face_count, "Face index is out of bounds");
		AR_ASSERT_MSG(index < faceCornerCount(face_index), "Edge index is out of bounds");

			return LineSegment::fromPoints(getCornerVert(face_index, index, transform), cgetCornerVert(face_index, index + 1, transform));
	}

	void Mesh::addVertex(Coord new_vert)
	{
		addVertex(new_vert, m_vertices.size());
	}
	
	void Mesh::addVertex(Coord new_vert, size_t index)
	{
		size_t prev_size = m_vertices.size();
		m_vertices.cResize(m_vertices.size() + 1);
		m_vertices.block(index + 1, 0, prev_size - index, 0) = m_vertices.block(index, 0, prev_size - index, 0);
		m_vertices[index] = new_vert;
	}
	
	void Mesh::addVertices(const Coords& new_verts)
	{
		addVertices(new_verts, m_vertices.size());
	}

	void Mesh::addVertices(const Coords& new_verts, size_t index)
	{
		size_t prev_size = m_vertices.size();
		
		m_vertices.cResize(m_vertices.size() + new_verts.size());
		m_vertices.segment(index + new_verts.size(), prev_size - index) = m_vertices.segment(index, prev_size - index);
		m_vertices.segment(index, new_verts.size()) = new_verts;
	}

	void Mesh::addFace(size_t start_vert, size_t index)
	{
		AR_ASSERT_MSG(index <= m_face_count, "Index is out of bounds");

		if (index == 0)
			m_faces.insert(m_faces.begin(), { start_vert, start_vert });
		else
			m_faces.insert(m_faces.begin() + firstIndexFromFace(index - 1) + faceCornerCount(index - 1) + 1, { start_vert, start_vert });
		
		m_face_count++;
	}

	void Mesh::addFace(const std::vector<size_t>& new_face, size_t index)
	{
		size_t last_indx = index != 0 ? lastIndexFromFace(index - 1) + 1 : 0;
		m_faces.reserve(new_face.size() + 1);
		m_faces.insert(m_faces.begin() + last_indx, new_face.begin(), new_face.end());
		m_faces.insert(m_faces.begin() + last_indx + new_face.size(), new_face[0]);
		m_face_count++;
	}

	void Mesh::extendFace(size_t face_index, size_t new_corner, size_t index)
	{
		AR_ASSERT_MSG(face_index < m_face_count, "Face index is out of bounds");
		AR_ASSERT_MSG(index <= faceCornerCount(face_index), "Corner index is out of bounds");

		#ifdef AR_DEBUG
		for (size_t i = 0; i < faceCornerCount(face_index); i++)
			AR_ASSERT_MSG(m_faces[firstIndexFromFace(face_index) + i] != new_corner, "Cannot have the same corner twice in a face");
		#endif

		if (index == 0) m_faces[lastIndexFromFace(face_index)] = new_corner;

		m_faces.insert(m_faces.begin() + firstIndexFromFace(face_index) + index, new_corner);
	}

	void Mesh::extendFace(size_t face_index, const std::vector<size_t>& new_corners, size_t index)
	{
		AR_ASSERT_MSG(face_index < m_face_count, "Face index is out of bounds");
		AR_ASSERT_MSG(index <= faceCornerCount(face_index), "Corner index is out of bounds");

		#ifdef AR_DEBUG
		for (size_t i = 0; i < new_corners.size(); i++)
			for (size_t j = 0; j < new_corners.size(); j++)
				AR_ASSERT_MSG(new_corners[i] != new_corners[j] && i != j, "Cannot have the same croner twice in a face");
		
		for (size_t i = 0; i < faceCornerCount(face_index); i++)
			for(size_t corner : new_corners)
				AR_ASSERT_MSG(m_faces[firstIndexFromFace(face_index) + i] != corner, "Cannot have the same corner twice in a face");
		#endif

		if (index == 0) m_faces[lastIndexFromFace(face_index)] = new_corners[0];

		m_faces.insert(m_faces.begin() + firstIndexFromFace(face_index) + index, new_corners.begin(), new_corners.end());
	}

	void Mesh::joinAsFace(size_t vert_start, size_t vert_stop)
	{
		AR_ASSERT_MSG(vert_start < vert_stop, "Starting index cannot be ahead of stopping index");
		AR_ASSERT_MSG(vert_start < vertCount() && vert_stop <= vertCount(), "Index is out of bounds");

		addFace(vert_stop - 1);

		for (size_t i = vert_start; i < vert_stop - 1; i++)
		{
			extendFace(faceCount() - 1, i);
		}
	}

	
	size_t Mesh::faceCornerCount(size_t face_index) const
	{
		AR_ASSERT_MSG(face_index < m_face_count, "Index is out of bounds");
		return lastIndexFromFace(face_index) - firstIndexFromFace(face_index);
	}

	void Mesh::removeVertex(size_t index)
	{
		removeVertices(index, 1);
	}

	// index_offset is the "width" of the segment being removed, so a value of 1 is the smallest value and will remove 1 element
	void Mesh::removeVertices(size_t index_begin, size_t index_offset)
	{
		AR_ASSERT_MSG(index_begin + index_offset <= m_vertices.size(),
			"Index range is out of bounds\nIndex: ", index_begin, "\nOffset: ", index_offset);

		size_t mov_size = m_vertices.size() - index_begin - index_offset;

		m_vertices.segment(index_begin, mov_size) = m_vertices.segment(index_begin + index_offset, mov_size);
		m_vertices.cResize(m_vertices.size() - index_offset);

		#ifdef AR_DEBUG
		for (size_t corner : m_faces)
			AR_ASSERT_MSG(corner < m_vertices.size(),
				"Face corner is no longer in bounds: ", corner);
		#endif
	}

	void Mesh::removeFace(size_t face_index)
	{
		AR_ASSERT_MSG(face_index < m_face_count, "Index is out of bounds");

		m_faces.erase(
			m_faces.begin() + firstIndexFromFace(face_index),
			m_faces.begin() + lastIndexFromFace(face_index) + 1);

		m_face_count--;
	}

	void Mesh::decreaseFace(size_t face_index, size_t index)
	{
		AR_ASSERT_MSG(face_index < m_face_count, "Face index is out of bounds");
		AR_ASSERT_MSG(index < faceCornerCount(face_index), "Corner index is out of bounds");

		if (faceCornerCount(face_index) == 1)
			removeFace(face_index);
		else
			m_faces.erase(m_faces.begin() + firstIndexFromFace(face_index) + index);
	}

	void Mesh::setVertex(size_t index, Coord new_val)
	{
		AR_ASSERT_MSG(index < m_vertices.size(), "Index is out of bounds");
		m_vertices[index] = new_val;
	}

	Mesh& Mesh::offset(Coord offset)
	{
		m_vertices.offset(offset);
		return *this;
	}

	Coord Mesh::getVertex(size_t index, const Transform& transform) const
	{
		AR_ASSERT_MSG(index < m_vertices.size(), "Index is out of bounds");
		return transform.applyTransform(m_vertices[index]);
	}

	Coord Mesh::getMedianVert() const
	{
		Coord avg_vert;

		for (const Coord& vert : m_vertices)
		{
			avg_vert += vert;
		}

		avg_vert /= (Real) vertCount();

		return avg_vert;
	}

	void Mesh::setCorner(size_t face_index, size_t index, size_t new_corner)
	{
		AR_ASSERT_MSG(face_index < m_face_count, "Face index is out of bounds");
		AR_ASSERT_MSG(index < faceCornerCount(face_index), "Corner index is out of bounds");

		if (index == 0)
		{
			m_faces[firstIndexFromFace(face_index)] = new_corner;
			m_faces[lastIndexFromFace(face_index)] = new_corner;
		}
		else
			m_faces[firstIndexFromFace(face_index) + index] = new_corner;
	}

	size_t Mesh::getCorner(size_t face_index, size_t index) const
	{
		AR_ASSERT_MSG(face_index < m_face_count, "Face index is out of bounds");
		AR_ASSERT_MSG(index < faceCornerCount(face_index), "Corner index is out of bounds");

		return m_faces[firstIndexFromFace(face_index) + index];
	}


	bool Mesh::isInside(const Coord& coord, const Transform& transform) const
	{
		Coord transformed_coord = transform.reverseTransform(coord);
		Line ray = Line::horzLine(transformed_coord);
		int winding_number = 0;

		for (size_t i = 0; i < faceCount(); i++)
		{
			for (size_t j = 0; j < faceCornerCount(i); j++)
			{
				LineSegment edge = getEdge(i, j);
				bool intersects = edge.intersects(ray);
 				if (intersects && !edge.isPerpendicular(ray))
				{
					Coord intersect = edge.intersect(ray);

					if (intersect.x < ray.offset.x)
						winding_number += edge.direction.y <= 0 ? 1 : -1;
					else if (intersects && intersect.x == ray.offset.x)
						winding_number += edge.intersects(transformed_coord) * (i > 0 ? -1 : 1);
				}
				else if (intersects)
					winding_number += edge.intersects(transformed_coord) * (i > 0 ? -1 : 1);
			}
		}

		return winding_number > 0;

	}

	bool Mesh::isInsideGrid(const Coord& coord, Real resolution, const Transform& transform) const
	{
		Coord transformed_coord = transform.reverseTransform(coord);
		Coord grid_coord(floor(transformed_coord.x, resolution), ceil(transformed_coord.y, resolution));

		Line raw_ray = Line::horzLine(transformed_coord);
		Line grid_ray = Line::horzLine(grid_coord);
		int winding_number = 0;

		for (size_t i = 0; i < faceCount(); i++)
		{
			for (size_t j = 0; j < faceCornerCount(i); j++)
			{
				LineSegment edge = getEdge(i, j);
				
				// calculate width based on the slope of the line
				Real width = (Real)std::sin(std::acos((edge.direction.dot(RealVertex(1, 0))) / (edge.direction.norm() * std::sqrt(1)))) * resolution;
				
				bool intersects = edge.intersects(grid_ray);
				if (intersects && !edge.isPerpendicular(grid_ray))
				{
					Coord intersect = static_cast<Line>(edge).intersect(raw_ray);

					if (intersect.x < raw_ray.offset.x + width / 2)
						winding_number += edge.direction.y <= 0 ? 1 : -1;
					else if (intersects && fcompare(intersect.x, raw_ray.offset.x, width))
						winding_number += edge.intersects(transformed_coord, width) * (i > 0 ? -1 : 1);
				}
				else if (intersects)
					winding_number += edge.intersects(transformed_coord, width) * (i > 0 ? -1 : 1);
			}
		}

		return winding_number > 0;
	}

	// Returns the first index of a face list
	size_t Mesh::firstIndexFromFace(size_t face_index) const
	{
		if (face_index == 0) return 0;

		size_t start_val = m_faces[0];
		size_t curr_face = 0;

		for (size_t i = 1; i < m_faces.size() - 1; i++)
		{
			if (m_faces[i] == start_val)
			{
				i++;
				start_val = m_faces[i];
				curr_face++;
				if(curr_face == face_index) return i;

			}
		}

		AR_ASSERT_VOLATILE(false, "Face index is out of bounds");
		return -1;
	}

	// Returns the last index for a face list including the repeating index
	size_t Mesh::lastIndexFromFace(size_t face_index) const
	{
		size_t start_val = m_faces[firstIndexFromFace(face_index)];
		for (size_t i = firstIndexFromFace(face_index) + 1; i < m_faces.size(); i++)
		{
			if (m_faces[i] == start_val) return i;
		}

		// This should never be hit
		AR_ASSERT_MSG(false, "A face list is missing an end index somehow");
		return -1;
	}

	// Streams the vertices and faces of a given mesh
	std::ostream& operator<<(std::ostream& stream, const Mesh& mesh)
	{
		stream << "VERTS: " << mesh.getVerts() << "\nFACES: ";
		
		if (mesh.faceCount() > 0)
		{
			stream << '\n';
			for (size_t i = 0; i < mesh.faceCount(); i++)
			{
				for (size_t j = 0; j < mesh.faceCornerCount(i); j++)
					stream << mesh.getCorner(i, j) << ',';
				stream << mesh.cgetCorner(i, mesh.faceCornerCount(i)) << '\n';
			}
		}
		else 
			stream << "None";

		return stream;
	}
}
