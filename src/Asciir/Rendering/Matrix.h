#pragma once

#include "Vertices.h"

namespace Asciir
{
	template<typename T>
	class Matrix
	{
	protected:
		T* m_data;
		arVertex m_size;


	public:

		Matrix(arVertex size = { 0, 0 });

		T& at(arVertex coord);
		T at(arVertex coord) const;

		T& at(size_t index);
		T at(size_t index) const;

		arVertex size() const;

		void resize(arVertex size);

		T& operator[](arVertex coord);
		T operator[](arVertex coord) const;

		T& operator[](size_t indx);
		T operator[](size_t indx) const;
	};

	template<typename T>
	std::ostream& operator<<(std::ostream& stream, const Matrix<T>& mat);
}

#include "Matrix.ipp"
