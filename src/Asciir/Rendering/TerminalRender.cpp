#include "arpch.h"
#include "TerminalRender.h"
#include "Asciir/Math/Lines.h"
#include "Asciir/Logging/Log.h"

namespace Asciir
{
	TerminalRender::TerminalRender(const std::string& title, size_t buffer_size)
		: m_title(title)
	{
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
			for (size_t i = 0; i < vertices.size() - 1; i++)
			{
				LineSegment line(vertices[i], vertices[i+1]);

				for (TInt i = 0; i <= line.length(); i++)
				{
					drawTile(line.at(i));
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

			for (TermVert vert : vertices)
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
					for (size_t verti = 0; verti < vertices.size() - 1; verti++)
					{
						LineSegment lsegment(vertices[verti], vertices[verti + 1]);

						if (lsegment.intersects({x, line}))
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
		#ifdef AR_DEBUG
		if (pos.x >= drawSize().x || pos.x < 0 || pos.y >= drawSize().y || pos.y < 0)
		{
			std::stringstream msg;
			msg << "Position " << pos << " is out of bounds. Bounds " << drawSize();
			throw std::runtime_error(msg.str());
		}
		#endif

		m_tiles[pos.x + m_tiles.size().x * pos.y] = m_tile_state;
	}

	Tile& TerminalRender::getTile(const TermVert& pos)
	{
		#ifdef AR_DEBUG
		if (pos.x >= drawSize().x || pos.x < 0 || pos.y >= drawSize().y || pos.y < 0)
		{
			std::stringstream msg;
			msg << "Position " << pos << " is out of bounds. Bounds " << drawSize();
			throw std::runtime_error(msg.str());
		}
		#endif

		return m_tiles[pos.x + m_tiles.size().x * (m_tiles.size().y - pos.y - 1)];
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

	void TerminalRender::resize(TermVert size)
	{

		#ifdef AR_DEBUG
		if (size.x > maxSize().x || size.x < 0 || size.y > maxSize().y ||size.y < 0)
		{
			std::stringstream msg;
			msg << "Size " << size << " is too large or negative. Max terminal size is " << maxSize();
			throw std::runtime_error(msg.str());
		}
		#endif

		m_should_resize = true;
		m_tiles.resize(size);
	}

	TRUpdateInfo TerminalRender::update()
	{

		TRUpdateInfo r_info;

		TermVert size = m_terminal_out.terminalSize();
		Coord pos = m_terminal_out.terminalPos();

		if (m_should_resize)
		{
			m_should_resize = false;
			std::string resize_str = "\x1b[?25l\x1b[8;" + std::to_string(m_tiles.size().y) + ';' + std::to_string(m_tiles.size().x) + 't';
			fwrite(resize_str.c_str(), 1, resize_str.size(), stderr);

			pushBuffer("\x1b[?25l");

			r_info.new_size = true;
		}
		else if (size.x != m_tiles.size().x || size.y != m_tiles.size().y)
		{
			pushBuffer("\x1b[?25l");
			m_tiles.resize(size);

			r_info.new_size = true;
		}

		if (m_pos.x != pos.x || m_pos.y != pos.y)
		{
			m_pos = pos;
			r_info.new_pos = true;
		}

		if (m_should_rename)
		{
			m_terminal_out.setTitle(m_title);
			m_should_rename = false;
			r_info.new_name = true;
		}

		return r_info;
	}

	void TerminalRender::draw()
	{
		pushBuffer("\x1b[H");

		m_terminal_out.clear();

		for (TInt y = 0; (size_t) y < m_tiles.size().y; y++)
		{
			for (TInt x = 0; (size_t) x < m_tiles.size().x; x++)
			{
				Tile& tile = m_tiles.get(TermVert(x, y));
				m_terminal_out.setForeground(tile.color);
				m_terminal_out.setBackground(tile.background_color);
				m_terminal_out.ansiCode<TerminalRender>(*this, x == 0);

				pushBuffer(tile.symbol);
				tile = Tile();
			}

			if((size_t) y < m_tiles.size().y - 1)
				pushBuffer('\n');
		}

		flushBuffer();
	}

	TRUpdateInfo TerminalRender::render()
	{
		TRUpdateInfo r_info = update();
		draw();

		return r_info;
	}

	TermVert TerminalRender::termSize() const
	{
		return m_terminal_out.terminalSize();
	}

	TermVert TerminalRender::drawSize() const
	{
		return static_cast<TermVert>(m_tiles.size());
	}
	
	TermVert TerminalRender::maxSize() const
	{
		
		return m_terminal_out.maxTerminalSize();
	}

	Coord TerminalRender::pos() const
	{
		WINDOWPLACEMENT placement = WINDOWPLACEMENT();
		GetWindowPlacement(GetConsoleWindow(), &placement);

		return { placement.rcNormalPosition.left, placement.rcNormalPosition.top };
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
		return m_terminal_out.attributes;
	}

	TerminalRender& TerminalRender::operator<<(const std::string& data)
	{
		pushBuffer(data);
		return *this;
	}

	TerminalRender& TerminalRender::operator<<(char data)
	{
		pushBuffer(data);
		return *this;
	}
}
