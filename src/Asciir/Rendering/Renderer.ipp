#include "Renderer.h"
#include "Asciir/Maths/Lines.h"

namespace Asciir
{
	template<typename TShader, std::enable_if_t<std::is_base_of_v<Shader2D, TShader>, bool>>
	void Renderer::submit(Ref<TShader> shader, Transform transform, int32_t z_order)
	{
		ShaderData data{ Ref<Shader2D>(shader), transform };

		// calculate visible quad, in order to truncate the area of which the shader will be drawn, to an area that will actually be visible 
		
		if (data.shader->size() != TermVert(-1, -1))
		{
			// TODO: optimize this if necessary
			Quad texture_quad = Quad(data.shader->size());

			Coord top_left_coord(size());
			Coord bottom_right_coord(0, 0);

			for (const Coord& vert : texture_quad.getVerts())
			{
				Coord transformed_vert = data.transform.applyTransform(vert);
				top_left_coord.x = std::min(top_left_coord.x, transformed_vert.x);
				top_left_coord.y = std::min(top_left_coord.y, transformed_vert.y);

				bottom_right_coord.x = std::max(bottom_right_coord.x, transformed_vert.x);
				bottom_right_coord.y = std::max(bottom_right_coord.y, transformed_vert.y);
			}

			// make sure the area is inside the terminal

			top_left_coord.x = top_left_coord.x < 0 ? 0 : floor(top_left_coord.x);
			top_left_coord.y = top_left_coord.y < 0 ? 0 : floor(top_left_coord.y);

			bottom_right_coord.x = ceil(bottom_right_coord.x);
			bottom_right_coord.x = bottom_right_coord.x >= (long long)size().x ? size().x : bottom_right_coord.x;
			bottom_right_coord.y = ceil(bottom_right_coord.y);
			bottom_right_coord.y = bottom_right_coord.y >= (long long)size().y ? size().y : bottom_right_coord.y;

			data.visible = Quad::fromCorners(top_left_coord, bottom_right_coord);
		}

		submitToQueue(QueueElem{ z_order, data });
	}
}