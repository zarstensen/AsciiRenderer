#include "Lines.h"

namespace Asciir
{
	template<typename T, std::enable_if_t<is_list_type_v<Line, T>, bool>>
	bool Line::visibleByAll(const T& lines, arVertex2D<Real> point)
	{
		for (const Line& line : lines)
			if (line.notVisible(point))
				return false;

		return true;
	}

	template<typename T, std::enable_if_t<is_list_type_v<Line, T>, bool>>
	bool Line::notVisibleByAll(const T& lines, arVertex2D<Real> point)
	{
		return !visibleByAll<T>(lines, point);
	}
}