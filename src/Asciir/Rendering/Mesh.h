#pragma once

#include "Asciir/Maths/Vertices.h"
#include "Asciir/Maths/Lines.h"

namespace Asciir
{
	// struct containing transform data for a mesh
	// TODO: reimplement transformation to only have one transformation matrix, and instead calculate it only when neccessarry, and reuse when possible.
	struct Transform
	{
		Coord applyTransform(const Coord& vec) const;
		Coord reverseTransform(const Coord& vec) const;

		// only set if used ???
		void setOrigin(Coord origin) { origin_transform = Eigen::Translation<Real, 2>(origin); }
		void setPos(Coord pos) { move_transform = Eigen::Translation<Real, 2>(pos); }
		void setScale(Scale2D scale) { scale_transform = Eigen::DiagonalMatrix<Real, 2>(scale); }
		void setRotation(Real rotation) { rotation_transform = Eigen::Rotation2D<Real>(rotation).toRotationMatrix(); }

		Eigen::Translation<Real, 2> origin_transform = Eigen::Translation<Real, 2>();

		Eigen::Translation<Real, 2> move_transform = Eigen::Translation<Real, 2>();

		Eigen::DiagonalMatrix<Real, 2> scale_transform = Eigen::DiagonalMatrix<Real, 2>({1, 1});

		Eigen::Matrix<Real, 2, 2> rotation_transform = Eigen::Rotation2D<Real>(0).toRotationMatrix();

	};

	static const Transform NoTransform = Transform();

	/// @brief A class containing vertices and data about how to connect them.  
	/// Points will be determinded wether to be outside or inside the mesh depending on the winding order of the edges
	///
	/// An index in a face list represents what vertex it should connect to, where the vertex is placed at the index in the vertices list
	/// A face consists of a list of indicies with the same index at the start and the end of the list (this defines the length of the list)
	/// Only the first index can appear twice in the list, other indicies inbetween must be unique.
	/// example: [0, 1, 2, 3, 0] (one face)
	/// example: [0,1,2,3,0, 3,2,0,1,3] (two faces)
	///
	/// error: [0, 1, 2, 3] (missing end *0*)
	/// error: [0, 1, 2, 1, 3, 0] (repeating index *1*)
	///
	/// The order of the indices in the list defines what winding order the edges will have,
	/// where a clockwise winding will be seen as an empty face (a hole) and an anti clockwise winding will be seen as full
	/// CCW: full
	/// CW: empty
	///
	/// when refering to a *face index*, the index of the face, relative to the other faces, not the list itself, should be used.
	/// meaning, if a face list of length 15 contains 3 faces, the maximum face index is 3, **NOT** 15.
	///
	class Mesh
	{
	protected: 
		Coords m_vertices;

		/// @brief stored in segments beginning and ending with an index representing a vertex.  
		// in each segment the vertices listed will be connected in the order they appear in the list.
		std::vector<size_t> m_faces;
		size_t m_face_count = 0;

	public:
		Mesh() = default;
		/// @brief Creates a mesh with edges connecting each vertex, and a single face.
		Mesh(const Coords& vertices);

		/// @brief Creates a mesh with edges connecting each vertex in each element.  
		// The first element in the vector will have the face connected as CW and the rest as CCW
		// NOTE: this constructor assumes that the vertices given are in a clockwise order
		Mesh(const std::vector<Coords>& polygon);
		/// @brief Creates a mesh containing the given vertices connected with the given edges.
		Mesh(const Coords& vertices, const std::vector<size_t>& edges);

		/// @brief returns the entire facelist for the mesh
		const std::vector<size_t>& getFaces() const { return m_faces; }

		/// @brief adds a vertex to the end of the vertices list.
		void addVertex(Coord new_vert);
		/// @brief adds a vertex, at the given index, in the vertices list.
		void addVertex(Coord new_vert, size_t index);

		/// @brief appends the given vertices to the vertices list
		void addVertices(const Coords& new_verts);
		/// @brief appends the given vertices at the give index
		void addVertices(const Coords& new_verts, size_t index);

		/// @brief same as addFace(size_t, size_t), face is appended to the end of the face list.
		void addFace(size_t start_vert = 0) { addFace(start_vert, m_face_count); };
		/// @brief adds a face with an area of 0 at position [start_vert]
		void addFace(size_t start_vert, size_t index);

		
		/// @brief adds a face using the given vertex index vector (meaning the indicies in the vector will be mapped to the corresponding vertices in the vertices list of the mesh, that will be used to create the face) as the corners.
		/// It will automaticly append the first value in the list to the end of the face list
		/// example: (input) [0, 1, 2] -> (face list) [0, 1, 2, 0]
		void addFace(const std::vector<size_t>& new_face) { addFace(new_face, faceCount()); };
		/// @brief same as addFace(const std::vector<size_t>&), but the face can be inserted at a custom index in the face list.
		void addFace(const std::vector<size_t>& new_face, size_t index);

		/// @brief inserts a corner at the given position and face, corner is automaticly added to the end of the face.
		void extendFace(size_t face_index, size_t new_corner) { extendFace(face_index, new_corner, faceCornerCount(face_index)); };
		/// @brief same as extendFace(size_t, size_t), except the index of the new corner in the face list, can be specified.
		void extendFace(size_t face_index, size_t new_corner, size_t corner_index);
		/// @brief same as extendFace(size_t, size_t), but multiple corners can be added at the same time.
		void extendFace(size_t face_index, const std::vector<size_t>& new_corners) { extendFace(face_index, new_corners, faceCornerCount(face_index)); }
		/// @brief same as extendFace(size_t, size_t, size_t), but multiple corners can be added at the same time.
		void extendFace(size_t face_index, const std::vector<size_t>& new_corners, size_t index);

		/// @brief creates a face (CCW) out of the range of vertices selecteds
		void joinAsFace(size_t vert_start, size_t vert_stop);
		/// @brief creates a face (CCW) out of the list of verticies specified
		void joinAsFace(const std::vector<size_t>& verts);

		/// @brief creates a hole (CW) out of the range of vertices selected
		void joinAsHole(size_t vert_start, size_t vert_stop);
		/// @brief creates a hole (CW) out of the list of verticies specified
		void joinAsHole(const std::vector<size_t>& verts);

		/// @brief same as removeVertices() but for a single index
		void removeVertex(size_t index);
		/// @brief removes the vertices in the range index_begin to index_offset
		/// @attention the face list is not modified by this operation, for a more consistent result use TODO: implement this thing
		void removeVertices(size_t index_begin, size_t index_offset);

		/// @brief removes the face at the given face index
		void removeFace(size_t face_index);
		/// @brief removes the given corner from the given face index
		void decreaseFace(size_t face_index, size_t index);

		/// @brief sets a new coordinate value to the given vertex index
		void setVertex(size_t index, Coord new_val);
		/// @brief offset all the vertices in the mesh by *offset*
		Mesh& offset(Coord offset);
		/// @brief get the vertex at the given index, using the passed transform.
		Coord getVertex(size_t index, const Transform& transform = NoTransform) const;
		/// @brief mods the input index by the number of vertices
		/// @see getVertex()
		Coord cgetVertex(size_t index, const Transform& transform = NoTransform) const { return getVertex(index % vertCount(), transform); };

		/// @brief get the centre vertex / centre point of the mesh
		Coord getMedianVert() const;

		/// @brief get list of vertices in the mesh
		const Coords& getVerts() const { return m_vertices; }

		/// @brief sets the new vertex index of the given face corner index.
		void setCorner(size_t face_index, size_t index, size_t new_corner);
		/// @brief gets the corners vertex index
		size_t getCorner(size_t face_index, size_t index) const;
		/// @brief mods the input index by the number of corners
		/// @see getCorner()
		size_t cgetCorner(size_t face_index, size_t index) const { return getCorner(face_index, index % faceCornerCount(face_index)); };
		/// @brief gets the corners vertex
		Coord getCornerVert(size_t face_index, size_t index, const Transform& transform = NoTransform) const { return getVertex(getCorner(face_index, index), transform); }
		/// @brief mods the input index by the number of corners
		/// @see getCornerVert()
		Coord cgetCornerVert(size_t face_index, size_t index, const Transform& transform = NoTransform) const { return getVertex(cgetCorner(face_index, index), transform); }
		
		/// @brief gets the linesegment representing the given face's edge at the given index.
		/// the index of a face edge is the same as the starting corner of the edge. 
		/// meaning, if a face has the vertices A, B, C, and D.
		/// the edge at index 0 will be the LineSegment between vertex A and B.
		/// index 1 will be B and C.
		/// index 2: C - D.
		/// index 4: D - A.
		LineSegment getEdge(size_t face_index, size_t index, const Transform& transform = NoTransform) const;
		/// @brief mods the index (not the face index) byt the number of corners in the face.
		/// @see getEdge()
		LineSegment cgetEdge(size_t face_index, size_t index, const Transform& transform = NoTransform) const { return getEdge(face_index, index % faceCornerCount(face_index), transform); };

		/// @brief gets the number of corners in a face.
		size_t faceCornerCount(size_t face_index) const;
		/// @brief gets the number of faces in the current mesh.
		size_t faceCount() const { return m_face_count; };
		/// @brief gets the total amount of corners in the current mesh.
		size_t cornerCount() const { return m_faces.size() - m_face_count; };
		/// @brief gets the number of vertices in the current mesh.
		size_t vertCount() const { return m_vertices.size(); }

		/// @brief determins wether the given coord will be inside the mesh, at the given transform.
		bool isInside(const Coord& coord, const Transform& transform = NoTransform) const;
		/// @brief same as isInside(), except the mesh will be fitted to a grid with the given resolution.  
		/// @note the fitting is performed **after** the mesh has been transformed.
		bool isInsideGrid(const Coord& coord, Real resolution, const Transform& transform = NoTransform) const;

	protected:

		/// @brief gets the starting index of the corresponding face, in the face list
		size_t firstIndexFromFace(size_t face_index) const;
		/// @brief gets the end index of the corresponding face, in the face list
		size_t lastIndexFromFace(size_t face_index) const;
	};

	/// @brief outputs the vertices and faces of a mesh to the given stream.
	std::ostream& operator<<(std::ostream& stream, const Mesh& mesh);
}
