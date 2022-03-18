#include "arpch.h"
#include "TerminalRenderer.h"
#include "Asciir/Maths/Lines.h"
#include "Asciir/Logging/Log.h"

#ifdef AR_WIN
#include "Asciir/Platform/Windows/WindowsARAttributes.h"
#elif defined AR_UNIX
#include "Asciir/Platform/Unix/UnixARAttributes.h"
#endif

// cast the TerminalRendererInterface this pointer to the current implementation,
// allowing access to platform dependent function to be called inside the TerminalRendererInterface class.
// should only be used inside the TerminalRendererInterface class
#define AR_IMPL(obj) (*((TerminalRenderer*)obj))

namespace Asciir
{
	size_t U8CharSize(const char* u8_str)
	{
		// check if the first bit in the first byte is 0, if so the char size is 1
		if (~(u8_str[0] >> 7) & 1U)
			return 1;
		// check if the first bits in the first bytes are 110 (size 2), 1110 (size 3) or 11110 (size 4)
		else
		{
			unsigned char check = 0b110;
			unsigned char mask = 0b111;
			for (int i = 2; i <= 4; i++)
			{
				if (!(bool)(((u8_str[0] >> (7 - i)) & mask) ^ check)) return i;

				check = (check << 1) + 0b10;
				mask = (mask << 1) + 0b1;
			}
		}

		// first bit was not formatted correctly
		AR_ASSERT_MSG(false, "Unknown utf-8 character");
		return 0;
	}

	size_t U8Len(const char* u8_str)
	{
		int len = 0;

		while (*u8_str)
			len += (*u8_str++ & 0xc0) != 0x80;

		return len;
	}

namespace TRInterface
{
	TerminalRendererInterface::TerminalRendererInterface(const TerminalRendererInterface::TerminalProps& term_props)
		: m_title(term_props.title), m_buffer(term_props.buffer_size), m_buff_stream(&m_buffer) {}

	void TerminalRendererInterface::initRenderer(const TerminalProps& term_props)
	{
		// TODO: no need for this to be a pointer
	#ifdef AR_WIN
		m_attr_handler = std::make_shared<WinARAttr>();
		AR_IMPL(this).update();
	#elif defined(AR_UNIX)
		m_attr_handler = std::make_shared<UnixARAttr>();
	#endif

		if (term_props.size != TermVert(0, 0))
			resize(term_props.size);

	}

	void TerminalRendererInterface::setColour(Colour colour)
	{
		m_tile_state.colour = colour;
	}

	void TerminalRendererInterface::setBackgroundColour(Colour colour)
	{
		m_tile_state.background_colour = colour;
	}

	void TerminalRendererInterface::setSymbol(char symbol)
	{
		m_tile_state.symbol = symbol;
	}

	void TerminalRendererInterface::drawVertices(const TermVerts& vertices, DrawMode mode)
	{
		if (mode == DrawMode::Line)
		{
			// loop over vertices in pairs and draw lines between them
			for (size_t i = 0; i < (size_t)vertices.size() - 1; i++)
			{
				Line line = Line::fromPoints(vertices[i], vertices[i + 1]);

				TInt length = (TInt)line.direction.norm();

				for (TInt j = 0; j <= length; j++)
				{
					auto point = line.pointFromGrid(j, 1);
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
					bool is_corner = false;
					bool tmp_inside = is_inside;
					for (size_t verti = 0; verti < (size_t)vertices.size(); verti++)
					{
						size_t next_vert = verti + 1 > (size_t) vertices.size() - 1 ? 0 : verti + 1;
						RealVertex point(x, line);
						LineSegment lsegment = LineSegment::fromPoints(vertices[verti], vertices[next_vert]);

						// corner case {jokes be funny :)}
						if ((RealVertex)vertices[verti] == point)
						{
							RealVertex other_corner = vertices[verti == 0 ? vertices.size() - 1 : verti - 1];

							LineSegment other_lsegment = LineSegment::fromPoints(other_corner, vertices[verti]);

							if (other_lsegment.direction.y == 0 || lsegment.direction.y == 0)
							{
								was_inside = is_inside;
								tmp_inside = !is_inside;
							}
							else if (other_lsegment.direction.y > 0 != lsegment.direction.y > 0)
							{
								is_corner = true;
								was_inside = false;
								tmp_inside = is_inside;
							}
							else
							{
								was_inside = true;
								tmp_inside = !is_inside;
							}

							break;
						}

						// calculate width based on the slope of the line
						Real width = (Real)std::sin(std::acos((lsegment.direction.dot(RealVertex(1, 0))) / (lsegment.direction.norm() * std::sqrt(1))));

						bool intersects = lsegment.intersects(point, width);
						if (intersects)
						{
							was_inside = true;
							tmp_inside = !tmp_inside;
						}
					}

					is_inside = tmp_inside;

					if (!(is_corner && is_inside) && (is_corner && !is_inside || is_inside || was_inside))
					{
						was_inside = false;
						drawTile({ x, line });
					}
				}
			}
		}
	}

	void TerminalRendererInterface::drawLine(const TermVert& a, const TermVert& b)
	{
		drawVertices({ a, b }, DrawMode::Line);
	}

	void TerminalRendererInterface::clearTerminal(Tile clear_tile)
	{
		for (size_t i = 0; i < (size_t)m_tiles.size(); i++)
			m_tiles.data()[i].current = clear_tile;
	}

	void TerminalRendererInterface::clearRenderTiles()
	{
		for (size_t i = 0; i < (size_t)m_tiles.size(); i++)
			m_tiles.data()[i].last = Tile::emptyTile();
	}

	void TerminalRendererInterface::setState(Tile tile)
	{
		m_tile_state = tile;
	}

	Tile& TerminalRendererInterface::getState()
	{
		return m_tile_state;
	}

	Tile TerminalRendererInterface::getState() const
	{
		return m_tile_state;
	}

	void TerminalRendererInterface::drawTile(const TermVert& pos)
	{
		drawTile(pos, m_tile_state);
	}

	void TerminalRendererInterface::drawTile(const TermVert& pos, const Tile& tile)
	{
		AR_ASSERT_MSG(pos.x < drawSize().x&& pos.x >= 0 && pos.y < drawSize().y&& pos.y >= 0,
			"Position ", pos, " is out of bounds. Bounds: ", drawSize());

		m_tiles(pos.x, pos.y).current = tile;
	}

	void TerminalRendererInterface::blendTile(const TermVert& pos)
	{
		blendTile(pos, m_tile_state);
	}

	void TerminalRendererInterface::blendTile(const TermVert& pos, const Tile& tile)
	{
		AR_ASSERT_MSG(pos.x < drawSize().x&& pos.x >= 0 && pos.y < drawSize().y&& pos.y >= 0,
			"Position ", pos, " is out of bounds. Bounds: ", drawSize());

		m_tiles(pos.x, pos.y).current.blend(tile);
	}

	TerminalRendererInterface::DrawTile& TerminalRendererInterface::getTile(const TermVert& pos)
	{
		AR_ASSERT_MSG(pos.x < drawSize().x&& pos.x >= 0 && pos.y < drawSize().y&& pos.y >= 0,
			"Position ", pos, " is out of bounds. Bounds: ", drawSize());

		return m_tiles(pos.x, pos.y);
	}

	void TerminalRendererInterface::setTitle(const std::string& title)
	{
		m_title = title;
		m_should_rename = true;
	}

	std::string TerminalRendererInterface::getTitle() const
	{
		return m_title;
	}

	const AsciiAttr& TerminalRendererInterface::getAttrHandler()
	{
		return *m_attr_handler;
	}

	void TerminalRendererInterface::resize(TermVert size)
	{
		AR_ASSERT_MSG(size.x <= AR_IMPL(this).maxSize().x && size.x > 0 && size.y <= AR_IMPL(this).maxSize().y && size.y > 0,
			"Size ", size, " is too large or negative. Max size: ", AR_IMPL(this).maxSize());

		m_should_resize = true;
		m_tiles.resize(size);
	}

	TerminalRendererInterface::TRUpdateInfo TerminalRendererInterface::update()
	{
		TRUpdateInfo r_info;

		TermVert size = AR_IMPL(this).termSize();
		Coord position = AR_IMPL(this).pos();

		// \x1b[?25l = hide cursor
		// the cursor will have to be rehidden every time the terminal gets resized
		if (m_should_resize)
		{
			// reset stored tiles from last update
			if (drawSize() != size)
				clearRenderTiles();

			m_should_resize = false;
			std::string resize_str = "\x1b[?25l\x1b[8;" + std::to_string(drawSize().y) + ';' + std::to_string(drawSize().x) + 't';
			fwrite(resize_str.c_str(), 1, resize_str.size(), stderr);

			r_info.new_size = true;
		}
		else if (size != drawSize())
		{
			m_buff_stream << "\x1b[?25l";
			m_tiles.resize(size);

			// reset stored tiles from last update
			clearRenderTiles();

			r_info.new_size = true;
		}

		if (m_pos != position)
		{
			m_pos = position;
			r_info.new_pos = true;
		}

		if (m_should_rename)
		{
			m_attr_handler->setTitle(std::cout, m_title);
			m_should_rename = false;
			r_info.new_name = true;
		}

		return r_info;
	}

	void TerminalRendererInterface::draw()
	{
		bool skipped_tile = false;

		m_attr_handler->clear();

		m_attr_handler->move({ 0, 0 });
		m_attr_handler->moveCode(getStream());

		// this loop needs to access the matrix as row first, then column, even though it is stored as column major,
		// as the terminal expects the buffer to be ordered as "row major", meaning newlines define where each row begins and ends.
		for (TInt y = 0; (size_t)y < drawSize().y; y++)
		{
			for (TInt x = 0; (size_t)x < drawSize().x; x++)
			{
				Tile& new_tile = m_tiles(x, y).current;
				Tile& old_tile = m_tiles(x, y).last;

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

				m_attr_handler->setForeground(new_tile.colour);
				m_attr_handler->setBackground(new_tile.background_colour);
				// the newline might not always be set, if the tile at position x == 0, is skipped
				m_attr_handler->ansiCode(getStream(), x == 0);

				if (!new_tile.eqAttr(old_tile) || new_tile.symbol != old_tile.symbol)
					m_buff_stream << (const char*)new_tile.symbol;

				old_tile = new_tile;
			}

			if ((size_t)y < (size_t)drawSize().y - 1 && !skipped_tile)
				m_buff_stream << '\n';
		}

		m_attr_handler->move(TermVert(drawSize().x - 1, drawSize().y - 1));
		m_attr_handler->moveCode(getStream());

		flushBuffer();
	}

	TerminalRendererInterface::TRUpdateInfo TerminalRendererInterface::render()
	{
		TRUpdateInfo r_info = update();
		draw();

		return r_info;
	}

	TermVert TerminalRendererInterface::drawSize() const
	{
		// x is the index for what matrix to acsess so y and z is equivalent to x and y.
		return (TermVert)m_tiles.dim();
	}

	void TerminalRendererInterface::pushBuffer(char c)
	{
		m_buffer.sputc(c);
	}

	void TerminalRendererInterface::pushBuffer(const std::string& str)
	{
		m_buffer.sputn(str.data(), str.size());
	}

	void TerminalRendererInterface::pushBuffer(const char* c_str)
	{
		m_buffer.sputn(c_str, strlen(c_str));
	}

	void TerminalRendererInterface::pushBuffer(const char* c_buff, size_t buff_len)
	{
		m_buffer.sputn(c_buff, buff_len);
	}

	void TerminalRendererInterface::flushBuffer()
	{
		m_buffer.pubsync();
	}

	std::array<bool, ATTR_COUNT>& TerminalRendererInterface::attributes()
	{
		return m_attr_handler->attributes;
	}
	
} // TRInterface

	std::ostream& operator<<(std::ostream& stream, const Tile& tile)
	{
		stream << (int)tile.symbol << " (" << tile.background_colour << ") (" << tile.colour << ") ";
		return stream;
	}
}
