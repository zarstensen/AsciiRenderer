#include "pch/arpch.h"
#include "Renderer.h"
#include "Asciir/Core/Engine.h"
#include "Asciir/Math/Lines.h"
#include "Asciir/Math/Vertices.h"

namespace Asciir
{

	TerminalRenderer* Renderer::s_renderer = nullptr;
	const AsciiAttr* Renderer::s_attr_handler = nullptr;
	std::vector<Renderer::QueueElem> Renderer::s_render_queue;

	void Renderer::init()
	{
		s_renderer = &AREngine::getEngine()->getTerminal().getRenderer();
		s_attr_handler = &s_renderer->getAttrHandler();
	}

	void Renderer::drawQueueElem(const QueueElem& q_elem)
	{
		Tile prev_state = s_renderer->getState();
		s_renderer->setState(q_elem.tile);
		
		Coord top_left_coord(size());
		Coord bottom_right_coord(0, 0);

		for (const Coord& vert : q_elem.mesh->getVerts())
		{
			Coord transformed_vert = q_elem.transform.applyTransform(vert);
			top_left_coord.x = std::min(top_left_coord.x, transformed_vert.x);
			top_left_coord.y = std::min(top_left_coord.y, transformed_vert.y);


			bottom_right_coord.x = std::max(bottom_right_coord.x, transformed_vert.x);
			bottom_right_coord.y = std::max(bottom_right_coord.y, transformed_vert.y);

		}
		
 		top_left_coord = top_left_coord;
		bottom_right_coord = bottom_right_coord;

		// make sure the area is inside the terminal

		top_left_coord.x = top_left_coord.x < 0 ? 0 : ceil(top_left_coord.x);
		top_left_coord.y = top_left_coord.y < 0 ? 0 : ceil(top_left_coord.y);

		bottom_right_coord.x = bottom_right_coord.x >= (long long) size().x ? size().x - 1 : floor(bottom_right_coord.x);
		bottom_right_coord.y = bottom_right_coord.y >= (long long) size().y ? size().y - 1 : floor(bottom_right_coord.y);
		
		for (Real y = top_left_coord.y; y < bottom_right_coord.y + 1; y++)
			for (Real x = top_left_coord.x; x < bottom_right_coord.x + 1; x++)
				if (q_elem.mesh->isInsideGrid({ x, y }, 1, q_elem.transform)) s_renderer->drawTile({ (TInt)x, (TInt)y });
				#ifdef AR_VISUALIZE_DRAW_BOX
				else drawTile({ x, y }, Tile(q_elem.tile.color.inverse()));
				#endif

		s_renderer->setState(prev_state);
	}

	

	void Renderer::submitToQueue(Ref<Mesh> mesh, Tile tile, Transform transform)
	{
		s_render_queue.push_back(QueueElem( mesh, tile, transform ));
		
		#if AR_RENDER_QUEUE_MAX != -1
		static_assert(AR_RENDER_QUEUE_MAX > 0);

		AR_ASSERT_VOLATILE(s_renderer_queue.capacity() > AR_RENDER_QUEUE_MAX,
			"Number of elements submitted to the render queue surpassed the maximum limit")

		#endif
	}

	Ref<Mesh> Renderer::submitRect(s_Coords<2> verts, Tile tile)
	{
		Ref<Mesh> rect = Mesh({ verts[0], {verts[1].x, verts[0].y }, verts[1], {verts[0].x, verts[1].x} });

		submitToQueue(rect, tile);

		return rect;
	}

	void Renderer::drawTile(Coord pos, Tile tile)
	{
		if (pos.x >= 0 && (size_t )pos.x < size().x && pos.y >= 0 && (size_t) pos.y < size().y)
		{
			Tile prev_state = s_renderer->getState();
			s_renderer->setState(tile);
			s_renderer->drawTile((TermVert) pos);
			s_renderer->setState(prev_state);
		}
	}

	Tile Renderer::viewTile(Coord pos)
	{
		AR_ASSERT_MSG(pos.x >= 0 && (size_t) pos.x < size().x && pos.y >= 0 && (size_t) pos.y < size().y, "Cannot view tile outside of terminal size");
		return s_renderer->getTile((TermVert)pos);
	}

	void Renderer::clear(Tile tile)
	{
		s_renderer->clearTerminal(tile);
	}

	void Renderer::resize(Size2D size)
	{
		s_renderer->resize(size);
	}
	
	Size2D Renderer::size()
	{
		return s_renderer->drawSize();
	}

	void Renderer::flushQueue()
	{
		for(QueueElem q_elem : s_render_queue)
		{
			drawQueueElem(q_elem);
		}

		size_t queue_size = s_render_queue.size();
		s_render_queue.clear();

		if (queue_size + AR_RENDER_QUEUE_MARGIN < s_render_queue.capacity())
		{
			size_t new_capacity = queue_size;

			#if AR_RENDER_QUEUE_MIN != -1
			#if AR_RENDER_QUEUE_MAX != -1
			static_assert(AR_RENDER_QUEUE_MIN + AR_RENDER_QUEUE_MARGIN < AR_RENDER_QUEUE_MAX);
			#endif
			static_assert(AR_RENDER_QUEUE_MIN > 0);
			
			new_capacity = std::max(new_capacity, (size_t) AR_RENDER_QUEUE_MIN);

			#endif

			s_render_queue.shrink_to_fit();
			s_render_queue.reserve(new_capacity);
		}
	}
}
