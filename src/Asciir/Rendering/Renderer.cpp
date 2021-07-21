#include "pch/arpch.h"
#include "Renderer.h"
#include "Asciir/Core/Engine.h"
#include "Asciir/Math/Lines.h"
#include "Asciir/Math/Vertices.h"

namespace Asciir
{
	TerminalRenderer* Renderer::s_renderer = nullptr;
	const AsciiAttr* Renderer::s_attr_handler = nullptr;
	std::vector<Renderer::QueueElem>* Renderer::s_submit_queue = new std::vector<Renderer::QueueElem>;
	std::vector<Renderer::QueueElem>* Renderer::s_render_queue = new std::vector<Renderer::QueueElem>;
	DeltaTime Renderer::s_min_dt = 0;

	void Renderer::init()
	{
		s_renderer = &AREngine::getEngine()->getTerminal().getRenderer();
		s_attr_handler = &s_renderer->getAttrHandler();
	}

	void Renderer::drawMeshData(const Renderer::MeshData& data)
	{
		s_renderer->setState(data.tile);

		Coord top_left_coord(size());
		Coord bottom_right_coord(0, 0);

		for (const Coord& vert : data.mesh->getVerts())
		{
			Coord transformed_vert = data.transform.applyTransform(vert);
			top_left_coord.x = std::min(top_left_coord.x, transformed_vert.x);
			top_left_coord.y = std::min(top_left_coord.y, transformed_vert.y);

			bottom_right_coord.x = std::max(bottom_right_coord.x, transformed_vert.x);
			bottom_right_coord.y = std::max(bottom_right_coord.y, transformed_vert.y);
		}

		top_left_coord = top_left_coord;
		bottom_right_coord = bottom_right_coord;

		// make sure the area is inside the terminal

		top_left_coord.x = top_left_coord.x < 0 ? 0 : floor(top_left_coord.x);
		top_left_coord.y = top_left_coord.y < 0 ? 0 : floor(top_left_coord.y);

		bottom_right_coord.x = ceil(bottom_right_coord.x) + 2;
		bottom_right_coord.x = bottom_right_coord.x >= (long long)size().x ? size().x - 2 : bottom_right_coord.x;
		bottom_right_coord.y = ceil(bottom_right_coord.y) + 2;
		bottom_right_coord.y = bottom_right_coord.y >= (long long)size().y ? size().y - 2 : bottom_right_coord.y;

		for (Real y = top_left_coord.y; y < bottom_right_coord.y; y++)
			for (Real x = top_left_coord.x; x < bottom_right_coord.x; x++)
				if (data.mesh->isInsideGrid({ x, y }, 1, data.transform)) s_renderer->blendTile(TermVert((TInt)x, (TInt)y));
#ifdef AR_VISUALIZE_DRAW_BOX
				else {
					s_renderer->setState(q_elem.tile.color.inverse());
					s_renderer->drawTile(TermVert(x, y));
					s_renderer->setState(q_elem.tile);
				}
#endif
	}

	void Renderer::drawShaderData(const ShaderData& data, const DeltaTime& time_since_start, const size_t& frames_since_start)
	{
		Mesh texture_quad = coordToQuad(data.shader->size());

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

		top_left_coord = top_left_coord;
		bottom_right_coord = bottom_right_coord;

		// make sure the area is inside the terminal

		top_left_coord.x = top_left_coord.x < 0 ? 0 : floor(top_left_coord.x);
		top_left_coord.y = top_left_coord.y < 0 ? 0 : floor(top_left_coord.y);

		bottom_right_coord.x = ceil(bottom_right_coord.x) + 2;
		bottom_right_coord.x = bottom_right_coord.x >= (long long)size().x ? size().x - 2 : bottom_right_coord.x;
		bottom_right_coord.y = ceil(bottom_right_coord.y) + 2;
		bottom_right_coord.y = bottom_right_coord.y >= (long long)size().y ? size().y - 2 : bottom_right_coord.y;

		for (Real y = top_left_coord.y; y < bottom_right_coord.y; y++)
			for (Real x = top_left_coord.x; x < bottom_right_coord.x; x++)
				if (texture_quad.isInsideGrid({ x, y }, 1, data.transform))
				{
					s_renderer->setState(data.shader->readTile(data.transform.reverseTransform({ x, y }), time_since_start, frames_since_start));
					s_renderer->blendTile(TermVert((TInt)x, (TInt)y));
				}
	}

	void Renderer::drawTileData(const TileData& data)
	{
		if (data.pos.x >= 0 && (size_t)data.pos.x < size().x && data.pos.y >= 0 && (size_t)data.pos.y < size().y)
		{
			s_renderer->setState(data.tile);
			s_renderer->blendTile((TermVert)data.pos);
		}
	}

	void Renderer::drawClearData(const ClearData& data)
	{
		AR_ASSERT_MSG(data.background_color.alpha == UCHAR_MAX, "Background color must be 100% opaque (alpha = 255), got: ", data.background_color.alpha, " as the alpha value");
		s_renderer->clearTerminal(data);
	}

	void Renderer::waitMinDT(DeltaTime curr_dt)
	{
		duration start_time = getTime();
		auto d_val = start_time + (duration)s_min_dt.nanoSeconds() - (duration)curr_dt.nanoSeconds();
		while (start_time + (duration)s_min_dt.nanoSeconds() - (duration)curr_dt.nanoSeconds() > getTime());
	}

	Mesh Renderer::coordToQuad(const Coord& coord)
	{
		return Mesh({ {0, 0}, {coord.x, 0}, coord, {0, coord.y} });
	}

	void Renderer::submitMesh(Ref<Mesh> mesh, Tile tile, Transform transform)
	{
		submitToQueue(QueueElem(MeshData{ mesh, tile, transform }));
	}

	void Renderer::submitShader(Ref<Shader> texture, Transform transform)
	{
		submitToQueue(ShaderData{ texture, transform });
	}

	Ref<Mesh> Renderer::submitRect(s_Coords<2> verts, Tile tile)
	{
		Ref<Mesh> rect = Mesh({ verts[0], {verts[1].x, verts[0].y }, verts[1], {verts[0].x, verts[1].x} });

		submitMesh(rect, tile);

		return rect;
	}

	void Renderer::submitTile(Coord pos, Tile tile)
	{
		submitToQueue(TileData{ pos, tile });
	}

	void Renderer::submitToQueue(QueueElem new_elem)
	{
		s_submit_queue->push_back(new_elem);

#if AR_RENDER_QUEUE_MAX != -1
		static_assert(AR_RENDER_QUEUE_MAX > 0);

		AR_ASSERT_VOLATILE(s_renderer_queue.capacity() > AR_RENDER_QUEUE_MAX,
			"Number of elements submitted to the render queue surpassed the maximum limit")
#endif
	}

	Tile Renderer::viewTile(Coord pos)
	{
		AR_ASSERT_MSG(pos.x >= 0 && (size_t)pos.x < size().x&& pos.y >= 0 && (size_t)pos.y < size().y, "Cannot view tile outside of terminal size");
		return s_renderer->getTile((TermVert)pos).current;
	}

	void Renderer::clear(Tile tile)
	{
		submitToQueue(tile);
	}

	void Renderer::resize(Size2D size)
	{
		s_renderer->resize(size);
	}

	Size2D Renderer::size()
	{
		return s_renderer->drawSize();
	}

	void Renderer::swapQueues()
	{
		std::swap(s_submit_queue, s_render_queue);

		size_t queue_size = s_render_queue->size();

		if (queue_size + AR_RENDER_QUEUE_MARGIN < s_submit_queue->capacity())
		{
			size_t new_capacity = queue_size;

#if AR_RENDER_QUEUE_MIN != -1

#if AR_RENDER_QUEUE_MAX != -1
			static_assert(AR_RENDER_QUEUE_MIN + AR_RENDER_QUEUE_MARGIN < AR_RENDER_QUEUE_MAX);
#endif

			static_assert(AR_RENDER_QUEUE_MIN > 0);

			new_capacity = std::max(new_capacity, (size_t)AR_RENDER_QUEUE_MIN);

#endif

			s_submit_queue->shrink_to_fit();
			s_submit_queue->reserve(new_capacity);
		}
	}

	void Renderer::flushRenderQueue(const DeltaTime& time_since_start, const size_t& frames_since_Start)
	{
		for (const QueueElem& q_elem : *s_render_queue)
		{
			switch (q_elem.index())
			{
			case 0:
				drawMeshData(std::get<MeshData>(q_elem));
				break;
			case 1:
				drawShaderData(std::get<ShaderData>(q_elem), time_since_start, frames_since_Start);
				break;
			case 2:
				drawTileData(std::get<TileData>(q_elem));
				break;
			case 3:
				drawClearData(std::get<ClearData>(q_elem));
				break;
			}
		}

		s_render_queue->clear();
	}
}