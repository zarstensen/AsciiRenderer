#include "Shader.h"

#include "Renderer.h"

namespace Asciir
{
	Coord Shader2D::toUV(const TermVert& coord)
	{
		if (size() != TermVert(-1, -1))
			return { coord.x / (Real)size().x, coord.y / (Real)size().y };
		else
			return { coord.x / (Real)Renderer::size().x, coord.y / (Real)Renderer::size().y };
	}
}
