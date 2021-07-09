#pragma once

#include "Asciir/Math/Vertices.h"
#include "Asciir/Math/Lines.h"

namespace Asciir
{
	// struct containing transform data for a mesh
	struct Transform
	{
		Coord pos = {0, 0};
		Scale2D scale = {1, 1};
		Real rotation = 0;

		Coord origin = {0, 0};

		Coord applyTransform(const Coord& vec) const;
		Coord reverseTransform(const Coord& vec) const;
	};

	static const Transform NoTransform = Transform();

	/*
	* A class containing vertices and data about how to connect them
	* Points will be determinded wether to be outside or inside the mesh depending on the winding order of the edges
	* 
	* An index in a face list represents what vertex it should connect to, where the vertex is placed at the index in the vertices list
	* A face consists of a list of indicies with the same index at the start and the end of the list (this defines the length of the list)
	* Only the first index can appear twice in the list, other indicies inbetween must be unique.
	* example: [0, 1, 2, 3, 0] (one face)
	* example: [0,1,2,3,0, 3,2,0,1,3] (two faces)
	*
	* error: [0, 1, 2, 3] (missing end *0*)
	* error: [0, 1, 2, 1, 3, 0] (repeating index *1*)
	*
	* The order of the indices in the list defines what winding order the edges will have,
	* Where a clockwise winding will be seen as an empty face and an anti clockwise winding will be seen as full
	* CCW: full
	* CW: empty
	*/
	class Mesh
	{
	protected:
		Coords m_vertices;

		// stored in segments beginning and ending with an index representing a vertex.
		// in each segment the vertices listed will be connected in the order they appear in the list.
		std::vector<size_t> m_faces;
		size_t m_face_count = 0;

	public:
		Mesh() = default;
		// Creates a mesh with edges connecting each vertex
		Mesh(const Coords& vertices);

		// Creates a mesh with edges connecting each vertex in each element
		// The first element in the vector will have the face connected as CW and the rest as CCW
		// NOTE: this constructor assumes that the vertices given are in a clockwise order
		Mesh(const std::vector<Coords>& polygon);
		Mesh(const Coords& vertices, const std::vector<size_t>& edges);

		const std::vector<size_t>& getFaces() const { return m_faces; }

		void addVertex(Coord new_vert);
		void addVertex(Coord new_vert, size_t index);

		void addVertices(const Coords& new_verts);
		void addVertices(const Coords& new_verts, size_t index);

		// adds a face with an area of 0 at position [start_vert]
		void addFace(size_t start_vert = 0) { addFace(start_vert, m_face_count); };
		void addFace(size_t start_vert, size_t index);
		
		/* 
		* adds a face using the given vector as the corners.
		* It will automaticly append the first value in the list to the end of the face list
		* example: (input) [0, 1, 2] -> (face list) [0, 1, 2, 0] 
		*/
		void addFace(const std::vector<size_t>& new_face) { addFace(new_face, faceCount()); };
		void addFace(const std::vector<size_t>& new_face, size_t index);
		
		// inserts a corner at the given position and face
		void extendFace(size_t face_index, size_t new_corner) { extendFace(face_index, new_corner, faceCornerCount(face_index)); };
		void extendFace(size_t face_index, size_t new_corner, size_t corner_index);
		void extendFace(size_t face_index, const std::vector<size_t>& new_corners) { extendFace(face_index, new_corners, faceCornerCount(face_index)); }
		void extendFace(size_t face_index, const std::vector<size_t>& new_corners, size_t index);

		// creates a face (CCW) out of the range of vertices selected
		void joinAsFace(size_t vert_start, size_t vert_stop);
		void joinAsFace(const std::vector<size_t>& verts);

		// creates a hole (CW) out of the range of vertices selected
		void joinAsHole(size_t vert_start, size_t vert_stop);
		void joinAsHole(const std::vector<size_t>& verts);

		void removeVertex(size_t index);
		void removeVertices(size_t index_begin, size_t index_offset);

		void removeFace(size_t face_index);
		void decreaseFace(size_t face_index, size_t index);
		
		void setVertex(size_t index, Coord new_val);
		Mesh& offset(Coord offset);
		Coord getVertex(size_t index, const Transform& transform = NoTransform) const;
		// mods the input index by the number of vertices
		Coord cgetVertex(size_t index, const Transform& transform = NoTransform) const { return getVertex(index % vertCount(), transform); };

		Coord getMedianVert() const;

		const Coords& getVerts() const { return m_vertices; }

		void setCorner(size_t face_index, size_t index, size_t new_corner);
		size_t getCorner(size_t face_index, size_t index) const;
		// mods the input index by the number of corners
		size_t cgetCorner(size_t face_index, size_t index) const { return getCorner(face_index, index % faceCornerCount(face_index));  };
		arVertex2D<Real> getCornerVert(size_t face_index, size_t index, const Transform& transform = NoTransform) const { return getVertex(getCorner(face_index, index), transform); }
		arVertex2D<Real> cgetCornerVert(size_t face_index, size_t index, const Transform& transform = NoTransform) const { return getVertex(cgetCorner(face_index, index), transform); }

		LineSegment getEdge(size_t face_index, size_t index, const Transform& transform = NoTransform) const;
		// mods the input index by the number of corners in a face, c = circular
		LineSegment cgetEdge(size_t face_index, size_t index, const Transform& transform = NoTransform) const { return getEdge(face_index, index % faceCornerCount(face_index), transform); };

		size_t faceCornerCount(size_t face_index) const;
		size_t faceCount() const { return m_face_count; };
		size_t cornerCount() const { return m_faces.size() - m_face_count; };
		size_t vertCount() const { return m_vertices.size(); }

		bool isInside(const Coord& coord, const Transform& transform = NoTransform) const;
		bool isInsideGrid(const Coord& coord, Real resolution, const Transform& transform = NoTransform) const;

	protected:

		size_t firstIndexFromFace(size_t face_index) const;
		size_t lastIndexFromFace(size_t face_index) const;
	};

	std::ostream& operator<<(std::ostream& stream, const Mesh& mesh);


}

