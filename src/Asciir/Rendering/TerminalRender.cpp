#include "arpch.h"
#include "TerminalRender.h"
#include "Lines.h"
#include "Asciir/Logging/Log.h"

namespace Asciir
{
	TerminalRender::TerminalRender()
	{
		update();
	}

	void TerminalRender::drawVertices(const arVertices& vertices, DrawMode mode)
	{
		if (mode == DrawMode::Line)
		{
			// loop over vertices in pairs and draw lines between them
			for (size_t i = 0; i < vertices.size() - 1; i++)
			{
				LineSegment line(vertices[i], vertices[i+1]);

				for (TInt i = 0; i <= line.length(); i++)
				{
					setTile(line.at(i), Tile(255));
				}
			}
		}
		else if (mode == DrawMode::Filled)
		{
			// use a scan line algorithm to draw the polygon
			TInt miny = SHRT_MAX;
			TInt maxy = 0;
			TInt minx = SHRT_MAX;
			TInt maxx = 0;

			for (arVertex vert : vertices)
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
						setTile({ x, line }, Tile(255));
					}
				}
			}

		}
	}

	void TerminalRender::setTile(const arVertex& pos, Tile tile)
	{

		#ifdef AR_DEBUG
		if (pos.x > size().x || pos.y > size().y)
		{
			std::stringstream msg;
			msg << "Position " << pos << " is out of bounds. Bounds " << size();
			throw std::runtime_error(msg.str());
		}
		#endif

		m_tiles[pos.x + m_tiles.size().x * (m_tiles.size().y - pos.y - 1)] = tile;
	}

	Tile& TerminalRender::getTile(const arVertex& pos)
	{
		#ifdef AR_DEBUG
		if (pos.x > size().x || pos.y > size().y)
		{
			std::stringstream msg;
			msg << "Position " << pos << " is out of bounds. Bounds " << size();
			throw std::runtime_error(msg.str());
		}
		#endif

		return m_tiles[pos.x + m_tiles.size().x * (m_tiles.size().y - pos.y - 1)];
	}

	void TerminalRender::update()
	{
		auto [width, height] = m_terminal.terminalSize();
		m_buffer.clear();

		if (width != m_tiles.size().x || height != m_tiles.size().y)
		{
			fwrite("\x1b[?25l", 1, 6, stderr);
			m_tiles.resize({ width, height });
			// width + 1 because of newline
			// +3 because of \x1b[H to go to the top left of terminal
			m_buffer.reserve(3 + (width + 1) * height * ATTR_MAX_SIZE);
		}
	}

	void TerminalRender::draw()
	{
		update();

		m_buffer += "\x1b[H";

		for (TInt y = 0; y < m_tiles.size().y; y++)
		{
			for (TInt x = 0; x < m_tiles.size().x; x++)
			{
				if (m_tiles[{x, y}].color != m_terminal.getBackground())
				{
					m_terminal.setBackground(m_tiles[{x, y}].color);
					m_terminal.ansiCode(m_buffer);
				}

				m_tiles[{x, y}] = Tile();
				m_buffer += ' ';
			}

			if(y < m_tiles.size().y - 1)
				m_buffer += '\n';
		}

		fwrite(m_buffer.c_str(), 1, m_buffer.size(), stderr);
	}

	arVertex Asciir::TerminalRender::size()
	{
		return m_tiles.size();
	}

	float TerminalRender::getSlope(const arVertex& a, const arVertex& b)
	{
		return float(a.y - b.y) / float(a.x - b.x);
	}
	void TerminalRender::drawLine(const arVertex& a, const arVertex& b)
	{
		drawVertices({ a, b }, DrawMode::Line);
	}
}
