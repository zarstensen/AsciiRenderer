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
		Color color;
	};

	class TerminalRender
	{
	protected:
		Matrix<Tile> m_tiles;
	
	public:
		TerminalRender();

		void drawVertices(const arVertices& vertices, DrawMode mode = DrawMode::Line);
		void setTile(const arVertex& pos, Tile tile);
		Tile& getTile(const arVertex& pos);

		void update();
		void draw();

		arVertex size();

		static float getSlope(const arVertex& a, const arVertex& b);
		void drawLine(const arVertex& a, const arVertex& b);

	private:
		AsciiAttr m_terminal;
		std::string m_buffer;


	};
}
