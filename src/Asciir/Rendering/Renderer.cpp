#include "pch/arpch.h"

#include "Renderer.h"
#include "Primitives.h"

#include "Asciir/Core/Application.h"
#include "Asciir/Maths/Lines.h"
#include "Asciir/Maths/Vertices.h"

#include <ChrTrc.h>

namespace Asciir
{
	TerminalRenderer* Renderer::s_renderer = nullptr;
	const AsciiAttr* Renderer::s_attr_handler = nullptr;
	std::vector<Renderer::QueueElem>* Renderer::s_submit_queue = new std::vector<Renderer::QueueElem>;
	std::vector<Renderer::QueueElem>* Renderer::s_render_queue = new std::vector<Renderer::QueueElem>;
	DeltaTime Renderer::s_min_dt = 0;

	void Renderer::init()
	{
		s_renderer = &ARApp::getApplication()->getTermRenderer();
		s_attr_handler = &s_renderer->getAttrHandler();
	}


	void Renderer::setThreads(uint32_t thread_count)
	{
		m_render_thread_pool.resize(thread_count, ETH::LThread(&renderThrd));

		for (ETH::LThread& thrd : m_render_thread_pool)
			thrd.start();
	}

	Tile Renderer::drawMeshData(Renderer::MeshData& data, const TermVert& coord)
	{
		if (data.visible.isInsideGrid(coord, 1) && data.mesh.isInsideGrid({ coord.x, coord.y }, 1))
			return data.tile;
		else
			return Tile::emptyTile();
	}

	Tile Renderer::drawShaderData(ShaderData& data, const TermVert& coord, const DeltaTime& time_since_start, size_t frames_since_start)
	{
		// shader has no bounds
		if (data.shader->size().x == -1 && data.shader->size().y == -1)
		{
			// TODO: what should the uv be here?
			return data.shader->readTile(data.transform.applyTransform(coord), Coord(1, 1), time_since_start, frames_since_start);
		}
		// shader has bounds, check if inside visible quad before doing anything else
		else if (data.visible.isInsideGrid(coord) && Quad(data.shader->size()).isInsideGrid(coord, data.transform))
		{
			Coord uv = Coord(
				coord.x / data.shader->size().x,
				coord.y / data.shader->size().y);

			return data.shader->readTile(data.transform.applyTransform(coord), uv, time_since_start, frames_since_start);
		}
		else
		{
			return Tile::emptyTile();
		}
	}

	Tile Renderer::drawTileData(TileData& data, const TermVert& coord)
	{
		if (data.pos == coord)
			return data.tile;
		else
			return Tile::emptyTile();
	}

	void Renderer::drawClearData(ClearData& data)
	{
		AR_ASSERT_MSG(data.background_colour.alpha == UCHAR_MAX, "Background colour must be 100% opaque (alpha = 255), got: ", data.background_colour.alpha, " as the alpha value");
		s_renderer->clearTerminal(data);
	}

	void Renderer::drawTile(TermVert tile_coord, const DeltaTime& dt, size_t df)
	{
		Tile result_tile = Tile::emptyTile();

		for (size_t i = 0; i < s_render_queue->size(); i++)
		{
			size_t ri = (s_render_queue->size() - 1 - i);
			switch (s_render_queue->at(ri).index())
			{
				case 0:
					result_tile = drawMeshData(std::get<MeshData>(s_render_queue->at(ri)), tile_coord).blend(result_tile);
					break;
				case 1:
					result_tile = drawShaderData(std::get<ShaderData>(s_render_queue->at(ri)), tile_coord, dt, df).blend(result_tile);
					break;
				case 2:
					result_tile = drawTileData(std::get<TileData>(s_render_queue->at(ri)), tile_coord).blend(result_tile);
					break;
				case 3:
					result_tile = std::get<ClearData>(s_render_queue->at(ri));
					break;
			}

			// if this is true, no matter what the next tile will be, it will have no effect on the final result, so just skip the rest of the render queue.
			if (!result_tile.is_empty && result_tile.background_colour.alpha == 255 && result_tile.colour.alpha == 255 && result_tile.symbol != '\0')
				break;
		}
		
		s_renderer->drawTile(tile_coord, result_tile);
	}

	void Renderer::waitMinDT(DeltaTime curr_dt)
	{
		duration start_time = getTime();
		auto d_val = start_time + (duration)s_min_dt.nanoSeconds() - (duration)curr_dt.nanoSeconds();
		while (start_time + (duration)s_min_dt.nanoSeconds() - (duration)curr_dt.nanoSeconds() > getTime());
	}

	// should this be a ref to mesh???
	void Renderer::submit(const Mesh& mesh, Tile tile, Transform transform)
	{
		MeshData data = MeshData{ mesh, tile };

		data.mesh.transform(transform);

		// calculate visible quad

		Coord top_left_coord(size());
		Coord bottom_right_coord(0, 0);

		for (const Coord& vert : data.mesh.getVerts())
		{
			top_left_coord.x = std::min(top_left_coord.x, vert.x);
			top_left_coord.y = std::min(top_left_coord.y, vert.y);

			bottom_right_coord.x = std::max(bottom_right_coord.x, vert.x);
			bottom_right_coord.y = std::max(bottom_right_coord.y, vert.y);
		}

		top_left_coord = top_left_coord;
		bottom_right_coord = bottom_right_coord;

		// make sure the area is inside the terminal

		top_left_coord.x = top_left_coord.x < 0 ? 0 : floor(top_left_coord.x);
		top_left_coord.y = top_left_coord.y < 0 ? 0 : floor(top_left_coord.y);

		bottom_right_coord.x = ceil(bottom_right_coord.x);
		bottom_right_coord.x = bottom_right_coord.x >= (long long)size().x ? size().x : bottom_right_coord.x;
		bottom_right_coord.y = ceil(bottom_right_coord.y);
		bottom_right_coord.y = bottom_right_coord.y >= (long long)size().y ? size().y : bottom_right_coord.y;

		data.visible = Quad::fromCorners(top_left_coord, bottom_right_coord);

		submitToQueue(data);
	}

	void Renderer::submit(Ref<Shader2D> shader, Transform transform)
	{
		ShaderData data{ shader, transform };

		// calculate visible quad
		if (shader->size() != TermVert(-1, -1))
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

		submitToQueue(data);
	}

	void Renderer::submitRect(s_Coords<2> verts, Tile tile)
	{
		Mesh rect = Mesh({ verts[0], {verts[1].x, verts[0].y }, verts[1], {verts[0].x, verts[1].x} });

		submit(rect, tile);
	}

	void Renderer::submit(TermVert pos, Tile tile)
	{
		submitToQueue(TileData{ tile, pos });
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

	Tile Renderer::viewTile(TermVert pos)
	{
		AR_ASSERT_MSG(pos.x >= 0 && (size_t)pos.x < size().x && pos.y >= 0 && (size_t)pos.y < size().y, "Cannot view tile outside of terminal size");
		return s_renderer->getTile((TermVert)pos).current;
	}

	Texture2D Renderer::grabScreen(TermVert rect_start, TermVert rect_offset)
	{
		CT_MEASURE_N("Grab Screen");

		// check for invalid arguments
		AR_ASSERT_MSG(rect_offset.x > 0 || rect_offset.x == -1 && rect_offset.y > 0 || rect_offset.y == -1,
			"Invalid grab screen region. rect_offset has invalid values: ", rect_offset);

		// grab region is out of bounds, return empty texture
		if (rect_start.x >= Renderer::size().x || rect_start.y >= Renderer::size().y)
			return {};

		// set -1 to the end of the terminal, or clamp the grab region to fit inside the terminal

		if (rect_offset.x == -1 || rect_start.x + rect_offset.x > Renderer::size().x)
			rect_offset.x = (TInt) Renderer::size().x - rect_start.x - 1;


		if (rect_offset.y == -1 || rect_start.y + rect_offset.y > Renderer::size().y)
			rect_offset.y = (TInt) Renderer::size().y - rect_start.y - 1;


		// grab the screen onto an Texture2D

		Texture2D result(rect_offset);

		for (TInt x = rect_start.x; x < rect_offset.x; x++)
		{
			for (TInt y = rect_start.y; y < rect_offset.y; y++)
			{
				AR_INFO(Renderer::size());
				result.setTile(Size2D(x, y), Renderer::viewTile(TermVert(x, y)));
			}
		}

		return result;
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

	void Renderer::flushRenderQueue(const DeltaTime& time_since_start, size_t frames_since_start)
	{
		AR_CORE_INFO("RENDER FRAME");
		// if only one thread is needed, avoid creating a seperate thread
		if ((uint32_t) s_renderer->drawSize().x * (uint32_t) s_renderer->drawSize().y <= thrd_tile_count || m_render_thread_pool.size() == 0)
		{
			for (TInt x = 0; x < s_renderer->drawSize().x; x++)
			{
				for (TInt y = 0; y < s_renderer->drawSize().y; y++)
				{
					drawTile(TermVert(x, y), time_since_start, frames_since_start);
				}
			}
		}
		else
		{
			uint32_t thrds = ((uint32_t)s_renderer->drawSize().x * (uint32_t)s_renderer->drawSize().y) / thrd_tile_count;

			// the thread count should not go above the thread pool size
			thrds = std::min((uint32_t) m_render_thread_pool.size(), thrds);

			AR_CORE_INFO("Using ", thrds, " threads to render the current frame!");
			
			m_avaliable_tile = 0;

			m_curr_dt = time_since_start;
			m_curr_df = frames_since_start;

			for (uint32_t i = 0; i < thrds; i++)
				m_render_thread_pool[i].startLoop();

			for (uint32_t i = 0; i < thrds; i++)
				m_render_thread_pool[i].joinLoop();
		}

		s_render_queue->clear();
	}

	void Renderer::renderThrd()
	{
		// should run until the avaliable tiles have run out
		while (true)
		{
			uint32_t current_tile;

			{
				std::unique_lock<std::mutex> lock(m_mutex);
				
				if (!(m_avaliable_tile < (uint32_t)s_renderer->drawSize().x * (uint32_t)s_renderer->drawSize().y))
					break;

				current_tile = m_avaliable_tile;
				m_avaliable_tile += thrd_tile_count;
			}

			// the loop should never go outside the draw range, so this is here to make sure it does not do that :)
			uint32_t end = std::min(current_tile + thrd_tile_count, (uint32_t) s_renderer->drawSize().x * (uint32_t) s_renderer->drawSize().y);

			for (uint32_t i = current_tile; i < end; i++)
			{
				//       calculate the x and y coordinates from the index
				drawTile(TermVert(i / s_renderer->drawSize().y, i % s_renderer->drawSize().y), m_curr_dt, m_curr_df);
			}
		}
	}

}