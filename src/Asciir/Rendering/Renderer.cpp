﻿#include "pch/arpch.h"

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
		// TODO: thread_count 1 should be the same as 0
		m_render_thread_pool.resize(thread_count, ETH::LThread(&renderThrd));

		for (ETH::LThread& thrd : m_render_thread_pool)
			thrd.start();
	}

	Tile Renderer::drawMeshData(Renderer::MeshData& data, TInt x, TInt y)
	{
		if (data.visible.isInsideGrid(Coord(x, y), 1) && data.mesh.isInsideGrid(Coord(x, y), 1))
			return data.tile;
		else
			return Tile::emptyTile();
	}

	Tile Renderer::drawShaderData(ShaderData& data, TInt x, TInt y, const DeltaTime& time_since_start, size_t frames_since_start)
	{
		// check if inside visible quad before doing anything else
		if (data.shader->size() == TermVert(-1, -1) || data.visible.isInsideGrid(Coord(x, y)) && Quad(data.shader->size()).isInsideGrid(Coord(x, y), data.transform))
		{
			return data.shader->readTile(data.transform.reverseTransformGrid({ x, y }), {x, y}, time_since_start, frames_since_start);
		}
		else
		{
			return Tile::emptyTile();
		}
	}

	Tile Renderer::drawTileData(TileData& data, TInt x, TInt y)
	{
		if (data.pos.x == x && data.pos.y == y)
			return data.tile;
		else
			return Tile::emptyTile();
	}

	void Renderer::drawClearData(ClearData& data)
	{
		AR_ASSERT_MSG(data.background_colour.alpha == UCHAR_MAX, "Background colour must be 100% opaque (alpha = 255), got: ", data.background_colour.alpha, " as the alpha value");
		s_renderer->clearTerminal(data);
	}

	void Renderer::drawTile(TInt x, TInt y, const DeltaTime& dt, size_t df)
	{
		Tile result_tile = Tile::emptyTile();

		for (size_t i = 0; i < s_render_queue->size(); i++)
		{
			size_t ri = (s_render_queue->size() - 1 - i);
			switch (s_render_queue->at(ri).index())
			{
				case 0: // mesh
					result_tile = Tile::blend(drawMeshData(std::get<MeshData>(s_render_queue->at(ri)), x, y), result_tile);
					break;
				case 1: // shader
					result_tile = Tile::blend(drawShaderData(std::get<ShaderData>(s_render_queue->at(ri)), x, y, dt, df), result_tile);
					break;
				case 2: // tile
					result_tile = Tile::blend(drawTileData(std::get<TileData>(s_render_queue->at(ri)), x, y), result_tile);
					break;
				case 3: // clear
					result_tile = Tile::blend(std::get<ClearData>(s_render_queue->at(ri)), result_tile);
					break;
			}

			// if this is true, no matter what the next tile will be, it will have no effect on the final result, so just skip the rest of the render queue.
			if (!result_tile.is_empty && result_tile.background_colour.alpha == 255 && result_tile.colour.alpha == 255 && result_tile.symbol != '\0')
				break;
		}
		
		s_renderer->drawTile(x, y, result_tile);
	}

	void Renderer::waitMinDT(DeltaTime curr_dt)
	{
		DeltaTime start_time = getTime();

		while (start_time + s_min_dt - curr_dt > getTime());
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

	void Renderer::submitRect(s_Coords<2> verts, Tile tile)
	{
		Mesh rect = Mesh({ verts[0], {verts[1].x, verts[0].y }, verts[1], {verts[0].x, verts[1].y} });

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

	Size2D Renderer::setResolution(Size2D res)
	{
		// TODO: error checking (new macro?)
		// retrieve the needed size informations
		TermVert resolution = s_renderer->monitorSize();
		TermVert console_size = s_renderer->termSize();

		// calculate the new font size

		TermVert font_size = TermVert(res).array() / console_size.array();
		TermVert max_size = resolution.array() / console_size.array();

		font_size.x = std::min(max_size.x, std::max(font_size.x, (TInt)1));
		font_size.y = std::min(max_size.y, std::max(font_size.y, (TInt)1));
		
		setFontSize(font_size);

		return Size2D(font_size).array() * size().array();
	}

	std::pair<Size2D, Size2D> Renderer::upscale(Size2D scale)
	{
		Size2D terminal_size = size();

		// check if the passed scale is usable
		if (scale.x == 0 || scale.y == 0 || terminal_size.x % scale.x != 0 || terminal_size.y % scale.y != 0)
		{
			AR_WARN("Upscale approximated the final size, due to the passed scale not dividing evently into the terminal size\n\tScale:", scale, "\n\tTSize: ", terminal_size);
		}

		terminal_size = terminal_size.array() / scale.array();

		Size2D font_size = getFontSize();

		font_size = font_size.array() * scale.array();

		resize(terminal_size);
		// TODO: error check?
		setFontSize(font_size);

		return { terminal_size, font_size };
	}

	std::pair<Size2D, Size2D> Renderer::downscale(Size2D scale)
	{
		Size2D font_size = getFontSize();

		if (scale.x == 0 || scale.y == 0 || font_size.x % scale.x != 0 || font_size.y % scale.y != 0)
		{
			AR_WARN("Downlscale approximated the final size, due to the passed scale not dividing evently into the font size\n\tScale:", scale, "\n\tFont: ", font_size);
		}

		font_size = font_size.array() / scale.array();
		
		Size2D terminal_size = size();
		terminal_size = terminal_size.array() * scale.array();

		resize(terminal_size);
		setFontSize(font_size);

		return {terminal_size, font_size};
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

	// TODO: width and height should be the width and height, assuming the update function has been called.
	TInt Renderer::width()
	{
		return (TInt)s_renderer->drawWidth();
	}

	TInt Renderer::height()
	{
		return (TInt)s_renderer->drawHeight();
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
		// if only one thread is needed, avoid creating a seperate thread
		if ((uint32_t) s_renderer->drawWidth() * (uint32_t) s_renderer->drawHeight() <= thrd_tile_count || m_render_thread_pool.size() == 0)
		{
			for (TInt x = 0; x < s_renderer->drawWidth(); x++)
			{
				for (TInt y = 0; y < s_renderer->drawHeight(); y++)
				{
					drawTile(x, y, time_since_start, frames_since_start);
				}
			}
		}
		else
		{
			uint32_t thrds = ((uint32_t)s_renderer->drawWidth() * (uint32_t)s_renderer->drawHeight()) / thrd_tile_count;

			// the thread count should not go above the thread pool size
			thrds = std::min((uint32_t) m_render_thread_pool.size(), thrds);

			AR_CORE_INFO("Using ", thrds, " threads to render the current frame");
			
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
				
				if (!(m_avaliable_tile < (uint32_t)s_renderer->drawWidth() * (uint32_t)s_renderer->drawHeight()))
					break;

				current_tile = m_avaliable_tile;
				m_avaliable_tile += thrd_tile_count;
			}

			// the loop should never go outside the draw range, so this is here to make sure it does not do that :)
			uint32_t end = std::min(current_tile + thrd_tile_count, (uint32_t) s_renderer->drawWidth() * (uint32_t) s_renderer->drawHeight());

			for (uint32_t i = current_tile; i < end; i++)
			{
				//       calculate the x and y coordinates from the index
				drawTile((TInt)(i % s_renderer->drawWidth()), (TInt)(i / s_renderer->drawWidth()), m_curr_dt, m_curr_df);
			}
		}
	}

}