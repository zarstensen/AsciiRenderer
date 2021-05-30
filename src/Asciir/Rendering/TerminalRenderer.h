#pragma once

#include "AsciiAttributes.h"
#include "RenderConsts.h"
#include "Asciir/Math/Vertices.h"
#include "Asciir/Math/Matrix.h"
#include "Asciir/Math/Tensor.h"

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

		Tile() = default;

		Tile(Color background_color, Color color, char symbol)
			: background_color(background_color), color(color), symbol(symbol) {}

		bool operator==(Tile other)
		{
			return background_color == other.background_color && color == other.color && symbol == other.symbol;
		}

		bool operator!=(Tile other)
		{
			return !(*this == other);
		}
	};

	std::ostream& operator<<(std::ostream& stream, const Tile& tile);

	

	class TerminalRenderer
	{
	public:
		struct TRUpdateInfo
		{
			bool new_size = false;
			bool new_pos = false;
			bool new_name = false;
		};

	protected:
		arTensor3D<Tile> m_tiles;
		Coord m_pos;
		Tile m_tile_state = Tile();
		std::string m_title;

	public:
		TerminalRenderer(const std::string& title, size_t buffer_size);

		void color(Color color);
		void backgroundColor(Color color);
		void symbol(char symbol);

		void drawVertices(const TermVerts& vertices, DrawMode mode = DrawMode::Line);
		void drawLine(const TermVert& a, const TermVert& b);

		void clearTerminal(Tile clear_tile = Tile());
		void clearTiles();

		void setState(Tile tile);
		Tile getState() const;
		Tile& getState();
		void drawTile(const TermVert& pos);
		Tile& getTile(const TermVert& pos);
		void setTitle(const std::string & title);
		std::string getTitle() const;

		const AsciiAttr& getAttrHandler();

		void resize(TermVert size);

		TRUpdateInfo update();
		void draw();
		TRUpdateInfo render();

		TermVert termSize() const;
		TermVert drawSize() const;
		TermVert maxSize() const;

		Coord pos() const;

		void pushBuffer(const std::string& data);
		void pushBuffer(char c);
		void flushBuffer();

		std::array<bool, ATTR_COUNT>& attributes();

	protected:
		std::shared_ptr<AsciiAttr> m_attr_handler;
		std::string m_buffer;
		bool m_should_resize = false;
		bool m_should_rename = true;

		friend AsciiAttr;
	};
	
}
