#pragma once

#include "Asciir/Maths/Vertices.h"
#include "Asciir/Maths/Lines.h"

namespace Asciir
{
	// struct containing transform data for a mesh
	// TODO: reimplement transformation to only have one transformation matrix, and instead calculate it only when neccessarry, and reuse when possible.

	/// @brief structure representing transformation data.
	/// 
	/// use setOrigin(), setPos(), setScale() and setRotation(), then the Transform structure itself will automaticly calculate the appropiate transformation matrix for the given values.
	/// 
	/// in order to apply the transformation, use applyTransform() and reverseTransform() on a coord;
	/// 
	/// implementation details:
	/// 
	/// the transformation stores both the transformations themselves as well as the transformation matrix.
	/// However, the transformation matrix is only calculated if needed, meaning a call to setOrigin for example, will not produce an entirely new transformation matrix.
	/// it is only on an transformation apply, that the matrix will be calculated.
	/// optionally, the matrix can also forcibly be calculated using the calcMat method, however this is highly discouraged.
	/// 
	struct Transform
	{
		Coord applyTransform(Coord vec);
		Coord applyTransform(TInt x, TInt y) { return applyTransform(Coord(x, y)); }
		/// @brief aplies the transform to a coordinate assumed to be in a grid of the specified resolution.
		/// here the coordinate is assumed to be the index of the cell in the grid, and from this, the midpoint of the cell is calculated and used in the transform.
		/// afterwards the result is re-offset from the cell origin to the cell index. The result is not rounded.
		Coord applyTransformGrid(Coord vec, Real resolution = 1)
		{
			// TODO: does this work?
			//return applyTransform(vec.array() + resolution / 2).array() - resolution / 2;
			return applyTransform(vec).floor(resolution);
		}
		
		Coord reverseTransform(Coord vec);
		Coord reverseTransform(TInt x, TInt y) { return reverseTransform(Coord(x, y)); }
		/// @see applyTransformGrid
		Coord reverseTransformGrid(Coord vec, Real resolution = 1)
		{
			// TODO: does this work?
			//return reverseTransform(vec.array() + resolution / 2).round(resolution).array() - resolution / 2;
			return reverseTransform(vec).floor(resolution);
		}


		/// @brief sets the origin of the transformation matrix.  
		/// only relevant for the scale and rotation part of the transformation matrix
		void setOrigin(Coord origin) { if (m_origin != origin) { m_has_mat = false; m_origin = origin; } }
		void setPos(Coord pos) { if (m_pos != pos) { m_has_mat = false; m_pos = pos; } }
		void setScale(Scale2D scale) { if (m_scale != scale) { m_has_mat = false; m_scale = scale; } }
		void setRotation(Real rotation) { if(!fequal(m_rotation, rotation)) { m_has_mat = false; m_rotation = rotation; } }

		const Coord& getOrigin() { return m_origin; }
		const Coord& getPos() { return m_pos; }
		const Scale2D& getScale() { return m_scale; }
		const Real& getRotation() { return m_rotation; }

		/// @brief calculates and stores the transformation matrix corresponding to the given transforms
		void calcMat();

	protected:

		typedef Eigen::Transform<Real, 2, Eigen::TransformTraits::Projective> TransformMat;

		// the transformation matrix should only have scale (1, 1) and nothing else
		TransformMat m_transform_matrix = TransformMat(Eigen::Scaling((Real)1));
		TransformMat m_inv_transform = TransformMat(Eigen::Scaling((Real)1));

		Coord m_origin;
		Coord m_pos;
		Scale2D m_scale = Scale2D(1, 1);
		Real m_rotation = 0;

		// this variable keeps track of wether the current transformation matrix corresponds to the stored attributes.
		bool m_has_mat = true;
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
		/// @brief applies the given transformation to the active mesh.
		void transform(Transform& transformation) { for (Coord& vert : m_vertices) vert = transformation.applyTransform(vert); }
		/// @brief applies the inverse of the passed transformation to the active mesh.
		void revertTransform(Transform& transformation) { for (Coord& vert : m_vertices) vert = transformation.reverseTransform(vert); }
		/// @brief get the vertex at the given index, using the passed transform.
		Coord& getVertex(size_t index);
		const Coord& getVertex(size_t index) const;
		/// @brief mods the input index by the number of vertices
		/// @see getVertex()
		Coord& cgetVertex(size_t index) { return getVertex(index % vertCount()); };
		const Coord& cgetVertex(size_t index) const { return getVertex(index % vertCount()); };

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
		Coord& getCornerVert(size_t face_index, size_t index) { return getVertex(getCorner(face_index, index)); }
		const Coord& getCornerVert(size_t face_index, size_t index) const { return getVertex(getCorner(face_index, index)); }
		/// @brief mods the input index by the number of corners
		/// @see getCornerVert()
		Coord& cgetCornerVert(size_t face_index, size_t index) { return getVertex(cgetCorner(face_index, index)); }
		const Coord& cgetCornerVert(size_t face_index, size_t index) const { return getVertex(cgetCorner(face_index, index)); }

		/// @brief gets the linesegment representing the given face's edge at the given index.
		/// the index of a face edge is the same as the starting corner of the edge. 
		/// meaning, if a face has the vertices A, B, C, and D.
		/// the edge at index 0 will be the LineSegment between vertex A and B.
		/// index 1 will be B and C.
		/// index 2: C - D.
		/// index 4: D - A.
		LineSegment getEdge(size_t face_index, size_t index) const;
		/// @brief mods the index (not the face index) byt the number of corners in the face.
		/// @see getEdge()
		LineSegment cgetEdge(size_t face_index, size_t index) const { return getEdge(face_index, index % faceCornerCount(face_index)); };

		/// @brief gets the number of corners in a face.
		size_t faceCornerCount(size_t face_index) const;
		/// @brief gets the number of faces in the current mesh.
		size_t faceCount() const { return m_face_count; };
		/// @brief gets the total amount of corners in the current mesh.
		size_t cornerCount() const { return m_faces.size() - m_face_count; };
		/// @brief gets the number of vertices in the current mesh.
		size_t vertCount() const { return m_vertices.size(); }

		/// @brief determins wether the given coord will be inside the mesh.  
		bool isInside(const Coord& coord) const;
		/// @brief same as isInside(), except the mesh will be fitted to a grid with the given resolution.  
		bool isInsideGrid(Coord coord, Real resolution) const;

	protected:

		/// @brief gets the starting index of the corresponding face, in the face list
		size_t firstIndexFromFace(size_t face_index) const;
		/// @brief gets the end index of the corresponding face, in the face list
		size_t lastIndexFromFace(size_t face_index) const;
	};

	/// @brief outputs the vertices and faces of a mesh to the given stream.
	std::ostream& operator<<(std::ostream& stream, const Mesh& mesh);
}
