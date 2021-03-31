#pragma once

#include "AsciiAttributes.h"
#include "Vertices.h"
#include "Matrix.h"

namespace Asciir
{

	enum class DrawMode
	{
		Line,
		Filled
	};

	struct Tile
	{
		Color background_color = BLACK8;
		Color color = WHITE8;
		char symbol = ' ';
	};

	class TerminalRender
	{
	protected:
		Matrix<Tile> m_tiles;

	public:
		TerminalRender(size_t buffer_size = 1024ULL * 1024ULL * 4ULL);

		void drawVertices(const arVertices& vertices, DrawMode mode = DrawMode::Line);
		void drawLine(const arVertex& a, const arVertex& b);
		void setTile(const arVertex& pos, Tile tile);
		Tile& getTile(const arVertex& pos);

		void resize(arVertex size);

		void update();
		void draw();

		arVertex size();

		void pushBuffer(const std::string& data);
		void pushBuffer(char c);
		void flushBuffer();


	protected:
		AsciiAttr m_terminal;
		std::string m_buffer;

		friend AsciiAttr;

		TerminalRender& operator<<(const std::string& data);
		TerminalRender& operator<<(char data);
	};
	
}
