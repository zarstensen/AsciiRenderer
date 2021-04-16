#pragma once

#include "Vertices.h"

namespace Asciir
{
	template<typename T>
	class Matrix
	{
	protected:
		T* m_data;
		arVertex<size_t> m_size;


	public:

		Matrix();
		Matrix(arVertex<size_t> size);

		T& at(arVertex<size_t> coord);
		T at(arVertex<size_t> coord) const;

		T& at(size_t index);
		T at(size_t index) const;

		arVertex<size_t> size() const;

		void resize(arVertex<size_t> size);

		T& operator[](arVertex<size_t> coord);
		T operator[](arVertex<size_t> coord) const;

		T& operator[](size_t indx);
		T operator[](size_t indx) const;
	};

	template<typename T>
	std::ostream& operator<<(std::ostream& stream, const Matrix<T>& mat);
}

#include "Matrix.ipp"
