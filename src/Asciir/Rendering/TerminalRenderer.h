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

	size_t U8CharSize(const char* u8_str);
	size_t U8Len(const char* u8_str);

	class UTF8Char
	{
	protected:
		static constexpr int UTF_CODE_LEN = 4;

		char m_data[UTF_CODE_LEN + 1] = { '\0' };

	public:

		UTF8Char() = default;

		UTF8Char(char c) { m_data[0] = c; };

		UTF8Char(const char* c)
		{
			AR_ASSERT_MSG(u8Len(c) < 2, "u8 string must only contain one or less characters, found ", u8Len(c));
			strcpy_s(m_data, c);
		};

		operator const char* () const
		{
			return m_data;
		}

		operator std::string() const
		{
			return (const char*)m_data;
		}

		explicit operator char* ()
		{
			return m_data;
		}

		explicit operator int() const
		{
			return *(int*)m_data;
		}

		bool operator==(const UTF8Char& other) const { return (int)*this == (int)other; }
		bool operator!=(const UTF8Char& other) const { return !(*this == other); }
		bool operator==(int c) const { return (int)*this == c; }
		bool operator!=(int c) const { return !(*this == c); }
		bool operator==(const char* c_str) const { return *this == UTF8Char(c_str); }
		bool operator!=(const char* c_str) const { return !(*this == c_str); }

	protected:
		static size_t u8Len(const char* u8_str)
		{
			int len = 0;

			while (*u8_str)
				len += (*u8_str++ & 0xc0) != 0x80;

			return len;
		}
	};

	struct Tile
	{
		UTF8Char symbol = ' ';
		Color color = WHITE8;
		Color background_color = BLACK8;
		bool is_empty = true;

		Tile(Color background_color = BLACK8, Color color = WHITE8, UTF8Char symbol = ' ')
			: symbol(symbol), color(color), background_color(background_color), is_empty(false) {}

		static Tile emptyTile()
		{
			Tile tile;
			tile.is_empty = true;
			return tile;
		}

		bool operator==(const Tile& other) const
		{
			if (!is_empty && !other.is_empty)
				return background_color == other.background_color && color == other.color && symbol == other.symbol;
			else
				return false;
		}

		bool operator!=(const Tile& other) const
		{
			if (!is_empty && !other.is_empty)
				return !(*this == other);
			else
				return false;
		}

		// blends the foreground and background color
		// the symbol is overwritten by the other tiles symbol, unless the symbol value is NULL ('\0')
		Tile& blend(const Tile& other)
		{
			if (other.symbol != '\0') symbol = other.symbol;
			// TODO should background color be blended here aswell?
			color.blend(other.color);
			background_color.blend(other.background_color);

			return *this;
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

		struct DrawTile
		{
			Tile current;
			Tile last = Tile::emptyTile();
		};

	protected:

		arMatrix<DrawTile> m_tiles;
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
		void clearRenderTiles();

		void setState(Tile tile);
		Tile getState() const;
		Tile& getState();
		// replaces the tile at pos with the current tile state
		void drawTile(const TermVert& pos);
		// blends the current tile state into the stored tile with the stored tile as the background
		void blendTile(const TermVert& pos);
		DrawTile& getTile(const TermVert& pos);
		void setTitle(const std::string& title);
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
		Ref<AsciiAttr> m_attr_handler;
		std::string m_buffer;
		bool m_should_resize = false;
		bool m_should_rename = true;

		friend AsciiAttr;
	};
}
