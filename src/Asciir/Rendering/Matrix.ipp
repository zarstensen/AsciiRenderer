#include "Matrix.h"

namespace Asciir
{
	template<typename T>
	Matrix<T>::Matrix()
		: m_data(nullptr), m_size({0, 0}) {}

	template<typename T>
	Matrix<T>::Matrix(arVertex<size_t> size)
	{
		resize(size);
	}

	template<typename T>
	T& Matrix<T>::at(arVertex<size_t> coord)
	{
		return m_data[coord.x + coord.y * m_size.x];
	}

	template<typename T>
	T Matrix<T>::at(arVertex<size_t> coord) const
	{
		return m_data[coord.x + coord.y * m_size.x];
	}

	template<typename T>
	T& Matrix<T>::at(size_t indx)
	{
		return m_data[indx];
	}

	template<typename T>
	T Matrix<T>::at(size_t indx) const
	{
		return m_data[indx];
	}

	template<typename T>
	arVertex<size_t> Matrix<T>::size() const
	{
		return m_size;
	}

	template<typename T>
	void Matrix<T>::resize(arVertex<size_t> size)
	{
		T* tmp_ptr = new T[size.x * size.y];
		std::fill(tmp_ptr, tmp_ptr + size.x * size.y, T());

		size_t cpy_amount = std::min(size.x, m_size.x) * sizeof(T);
		size_t loop_amount = std::min(size.y, m_size.y);

		for (size_t i = 0; i < loop_amount; i++)
		{
			memcpy(tmp_ptr + size.x * i, m_data + m_size.x * i, cpy_amount);
		}

		m_size = size;
		std::swap(m_data, tmp_ptr);

		delete[] tmp_ptr;
	}


	template<typename T>
	T& Matrix<T>::operator[](arVertex<size_t> coord)
	{
		return at(coord);
	}

	template<typename T>
	T Matrix<T>::operator[](arVertex<size_t> coord) const
	{
		return at(coord);
	}

	template<typename T>
	T& Matrix<T>::operator[](size_t indx)
	{
		return at(indx);
	}

	template<typename T>
	T Matrix<T>::operator[](size_t indx) const
	{
		return at(indx);
	}


	template<typename T>
	std::ostream& operator<<(std::ostream& stream, const Matrix<T>& mat)
	{
		for (size_t i = 0; i < mat.size().x * mat.size().y; i++)
		{
			stream << mat.at(i);

			if (i % mat.size().x == mat.size().x - 1)
				stream << '\n';
			else
				stream << ',';
		}

		return stream;
	}
}
