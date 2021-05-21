#include "arpch.h"
#include "TerminalRender.h"
#include "Asciir/Math/Lines.h"
#include "Asciir/Logging/Log.h"

#ifdef AR_WIN
#include "Asciir/Platform/Windows/WindowsARAttributes.h"
#elif defined AR_UNIX
#include "Asciir/Platform/Unix/UnixARAttributes.h"
#endif

namespace Asciir
{
	TerminalRender::TerminalRender(const std::string& title, size_t buffer_size)
		: m_title(title)
	{

		#ifdef AR_WIN
		m_attr_handler = std::make_unique<WinARAttr>();
		#elif defined AR_UNIX
		m_attr_handler = std::make_unique<UnixARAttr>();
		#endif

		m_buffer.reserve(buffer_size);
		update();
	}

	void TerminalRender::color(Color color)
	{
		m_tile_state.color = color;
	}

	void TerminalRender::backgroundColor(Color color)
	{
		m_tile_state.background_color = color;
	}

	void TerminalRender::symbol(char symbol)
	{
		m_tile_state.symbol = symbol;
	}

	void TerminalRender::drawVertices(const TermVerts& vertices, DrawMode mode)
	{
		if (mode == DrawMode::Line)
		{
			// loop over vertices in pairs and draw lines between them
			for (size_t i = 0; i < (size_t) vertices.size() - 1; i++)
			{
				LineSegment line(vertices[i], vertices[i+1]);
				
				TInt length = line.length();

				for (TInt j = 0; j <= length; j++)
				{
					auto point = line.at(j);
					drawTile(point);
				}
			}
		}
		else if (mode == DrawMode::Filled)
		{
			// use a scan line algorithm to fill the polygon
			TInt miny = SHRT_MAX;
			TInt maxy = 0;
			TInt minx = SHRT_MAX;
			TInt maxx = 0;

			for (const TermVert& vert : vertices)
			{
				miny = std::min(vert.y, miny);
				maxy = std::max(vert.y, maxy);
				minx = std::min(vert.x, minx);
				maxx = std::max(vert.x, maxx);
			}


			for (TInt line = miny; line < maxy + 1; line++)
			{
				bool is_inside = false;
				bool was_inside = false;

				for (TInt x = minx; x < maxx + 1; x++)
				{
					for (size_t verti = 0; verti < (size_t) vertices.size() - 1; verti++)
					{
						LineSegment lsegment(vertices[verti], vertices[verti + 1]);
						bool intersects = lsegment.intersects({ x, line });
						if (intersects)
						{
							was_inside = true;
							is_inside = !is_inside;
						}

					}

					LineSegment lsegment(vertices[vertices.size() - 1], vertices[0]);

					if (lsegment.intersects({ x, line }))
					{
						was_inside = true;
						is_inside = !is_inside;
					}

					if (is_inside || was_inside)
					{
						was_inside = false;
						drawTile({ x, line });
					}
				}
			}

		}
	}
	
	void TerminalRender::drawLine(const TermVert& a, const TermVert& b)
	{
		drawVertices({ a, b }, DrawMode::Line);
	}

	void TerminalRender::clearTerminal(Tile clear_tile)
	{
		m_tiles.block(0, 0, drawSize().x, drawSize().y).fill(clear_tile);
	}

	void TerminalRender::clearTiles()
	{
		m_tiles.fill(Tile());
	}

	void TerminalRender::setState(Tile tile)
	{
		m_tile_state = tile;
	}

	Tile& TerminalRender::getState()
	{
		return m_tile_state;
	}

	Tile TerminalRender::getState() const
	{
		return m_tile_state;
	}

	void TerminalRender::drawTile(const TermVert& pos)
	{
		AR_ASSERT_MSG(pos.x < drawSize().x && pos.x >= 0 && pos.y < drawSize().y && pos.y >= 0,
					 "Position ", pos, " is out of bounds. Bounds: ", drawSize());

		m_tiles(0, pos.x, pos.y) = m_tile_state;
	}

	Tile& TerminalRender::getTile(const TermVert& pos)
	{
		AR_ASSERT_MSG(pos.x < drawSize().x && pos.x >= 0 && pos.y < drawSize().y && pos.y >= 0,
			"Position ", pos, " is out of bounds. Bounds: ", drawSize());

		return m_tiles(0, pos.x, pos.y);
	}

	void TerminalRender::setTitle(const std::string& title)
	{
		m_title = title;
		m_should_rename = true;
	}

	std::string TerminalRender::getTitle() const
	{
		return m_title;
	}

	AsciiAttr& TerminalRender::getAttrHandler()
	{
		return *m_attr_handler;
	}

	void TerminalRender::resize(TermVert size)
	{
		AR_ASSERT_MSG(size.x <= maxSize().x && size.x > 0 && size.y <= maxSize().y && size.y > 0,
			          "Size ", size, " is too large or negative. Max size: ", maxSize());

		m_should_resize = true;
		m_tiles.resize({ (size_t) 2, (size_t) size.x, (size_t) size.y });
	}

	TRUpdateInfo TerminalRender::update()
	{

		TRUpdateInfo r_info;

		TermVert size = m_attr_handler->terminalSize();
		Coord pos = m_attr_handler->terminalPos();

		// \x1b[?25l = hide cursor
		// the cursor will have to be rehidden every time the terminal gets resized
		if (m_should_resize)
		{
			m_should_resize = false;
			std::string resize_str = "\x1b[?25l\x1b[8;" + std::to_string(drawSize().y) + ';' + std::to_string(drawSize().x) + 't';
			fwrite(resize_str.c_str(), 1, resize_str.size(), stderr);

			// reset stored tiles from last update
			clearTiles();

			r_info.new_size = true;
		}
		else if (size.x != drawSize().x || size.y != drawSize().y)
		{
			pushBuffer("\x1b[?25l");
			m_tiles.resize({ 2, (size_t)size.x, (size_t)size.y });

			// reset stored tiles from last update
			clearTiles();

			r_info.new_size = true;
		}

		if (m_pos.x != pos.x || m_pos.y != pos.y)
		{
			m_pos = pos;
			r_info.new_pos = true;
		}

		if (m_should_rename)
		{
			m_attr_handler->setTitle(m_title);
			m_should_rename = false;
			r_info.new_name = true;
		}

		return r_info;
	}

	void TerminalRender::draw()
	{
		m_attr_handler->clear();

		bool skipped_tile = false;

		m_attr_handler->move({ 0, 0 });
		m_attr_handler->moveCode(*this);
		
		for (TInt y = 0; (size_t) y < drawSize().y; y++)
		{
			for (TInt x = 0; (size_t) x < drawSize().x; x++)
			{
				Tile& new_tile = m_tiles(0, x, y );
				Tile& old_tile = m_tiles(1, x, y );

				if (new_tile == old_tile)
				{
					skipped_tile = true;
					continue;
				}

				if (skipped_tile)
				{
					skipped_tile = false;
					m_attr_handler->move({ x, y });
				}

				m_attr_handler->setForeground(new_tile.color);
				m_attr_handler->setBackground(new_tile.background_color);
				m_attr_handler->ansiCode(*this, x == 0);

				pushBuffer(new_tile.symbol);
			}

			if((size_t) y < (size_t) drawSize().y - 1 && !skipped_tile)
				pushBuffer('\n');
		}

		m_attr_handler->move({ drawSize().x - 1, drawSize().y - 1 });
		m_attr_handler->moveCode(*this);

		flushBuffer();

		auto tile_block = m_tiles.block(0, 0, drawSize().x, drawSize().y);
		m_tiles.block(m_tiles.size().y, 0, drawSize().x, drawSize().y) = tile_block;
	}

	TRUpdateInfo TerminalRender::render()
	{
		TRUpdateInfo r_info = update();
		draw();

		return r_info;
	}

	TermVert TerminalRender::termSize() const
	{
		return m_attr_handler->terminalSize();
	}

	TermVert TerminalRender::drawSize() const
	{
		// x is the index for what matrix to acsess so y and z is equivalent to x and y.
		return { (TInt) m_tiles.size().y, (TInt) m_tiles.size().z};
	}
	
	TermVert TerminalRender::maxSize() const
	{
		
		return m_attr_handler->maxTerminalSize();
	}

	Coord TerminalRender::pos() const
	{
		return m_attr_handler->terminalPos();
	}

	void TerminalRender::pushBuffer(char c)
	{
		if (m_buffer.size() + 1 > m_buffer.capacity())
			flushBuffer();

		m_buffer += c;
	}


	void TerminalRender::pushBuffer(const std::string& data)
	{
		if (data.size() + m_buffer.size() > m_buffer.capacity())
			flushBuffer();
		
		if (data.size() > m_buffer.capacity())
			fwrite(data.c_str(), 1, data.size(), stdout);

		m_buffer += data;
	}

	void TerminalRender::flushBuffer()
	{
		fwrite(m_buffer.c_str(), 1, m_buffer.size(), stdout);
		m_buffer.clear();
	}

	std::array<bool, ATTR_COUNT>& TerminalRender::attributes()
	{
		return m_attr_handler->attributes;
	}
	
	std::ostream& operator<<(std::ostream& stream, const Tile& tile)
	{
		stream << (int)tile.symbol;
		return stream;
	}
}
