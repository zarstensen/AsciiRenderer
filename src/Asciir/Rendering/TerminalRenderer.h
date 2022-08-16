#pragma once

#include "AsciiAttributes.h"
#include "RenderConsts.h"
#include "Asciir/Maths/Vertices.h"
#include "Asciir/Maths/Matrix.h"
#include "Asciir/Maths/Tensor.h"

namespace Asciir
{
	// ============ UTF8Char ============
	
	/// @brief the draw mode for the TerminalRenderer
	enum class DrawMode
	{
		Line,
		Filled
	};

	/// @brief checks the size of a single UTF-8 character
	size_t U8CharSize(const char* u8_str);
	/// @brief calculates the UTF-8 character count from the given string
	size_t U8Len(const char* u8_str);

	/// @brief stores a singular UTF 8 character.
	class UTF8Char
	{
	protected:
		/// @brief the maximum size of a UTF8 character, in bytes
		static constexpr int UTF_CODE_LEN = 4;

		char m_data[UTF_CODE_LEN + 1] = { '\0' };

	public:

		UTF8Char() = default;

		/// @brief constructs a UTF8Char from the given character
		UTF8Char(char c) { m_data[0] = c; };
		/// @brief constructs a UTF8Char from the given wide character
		UTF8Char(wchar_t wc) { *this = fromCode((uint32_t) wc); }

		/// @brief constructs a UTF8Char instance from the given UTF-8 binary sequence
		UTF8Char(const char* c)
		{
			AR_ASSERT_MSG(U8Len(c) < 2, "u8 string must only contain one or less characters, found ", U8Len(c));

		#ifdef AR_WIN
			strcpy_s(m_data, c);
		#else
			strcpy(m_data, c);
		#endif
		};


		/// @brief constructs a UTF8Char from the given UTF8 decimal code
		/// @important ONLY use this for decimal codes and not the actual binary representation of the UTF-8 character, as this converts the decimal code to the binary representation.
		/// instead use the constructor that takes a const char* UTF8Char(const char*).
		static UTF8Char fromCode(uint32_t code_point);

		/// @brief retrieves a null terminated c_str containing the UTF-8 character
		operator const char* () const
		{
			return m_data;
		}

		/// @brief retrieves a std::string containing the UTF-8 charcter
		operator std::string() const
		{
			return (const char*)m_data;
		}

		explicit operator char* ()
		{
			return m_data;
		}

		/// @brief retrieve the integer representing the UTF-8 character
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
	};

	// ============ Tile ============

	struct Tile
	{
		UTF8Char symbol = ' ';
		Colour colour = WHITE8;
		Colour background_colour = BLACK8;
		bool is_empty = true;

		Tile(Colour background_colour = BLACK8, Colour colour = WHITE8, UTF8Char symbol = ' ', bool is_empty = false)
			: symbol(symbol), colour(colour), background_colour(background_colour), is_empty(is_empty) {}

		static inline Tile emptyTile()
		{
			return Tile(BLACK8, WHITE8, ' ', true);
		}

		/// @brief checks if the attribues of the tile are all equal.
		bool eqAttr(const Tile& other)
		{
			return !is_empty && !other.is_empty && colour == other.colour && background_colour == other.background_colour;
		}

		bool operator==(const Tile& other) const
		{
			return !is_empty && !other.is_empty && background_colour == other.background_colour && colour == other.colour && symbol == other.symbol;
		}

		bool operator!=(const Tile& other) const
		{
			return !(*this == other);
		}

		// blends the foreground and background colour
		// the symbol is overwritten by the other tiles symbol, unless the symbol value is NULL ('\0')
		Tile& blend(const Tile& other)
		{
			// tile is empty, nothing should be done
			if (other.is_empty)
			{
				return *this;
			}
			else if (is_empty)
			{
				*this = other;
				return *this;
			}

			if (other.symbol != '\0') symbol = other.symbol;
			// TODO should background colour be blended here aswell?
			colour.blend(other.colour);
			background_colour.blend(other.background_colour);

			return *this;
		}

		/// @brief static version of blend(Tile)
		static Tile blend(const Tile& background, const Tile& foreground)
		{
			Tile result = background;
			result.blend(foreground);

			return result;
		}
	};

	std::ostream& operator<<(std::ostream& stream, const Tile& tile);

	// ==================== TerminalRenderer ====================

	namespace TRInterface
	{

		/// @brief console output buffer for the TerminalRenderer pushBuffer functions.  
		/// 
		/// should only be used by the TerminalRenderer and nothing else.  
		/// implementation is latform specific
		/// 
		/// implementation details:
		/// 
		/// whenever xsputn is called, the buffer checks if there is enough space for the passed value.
		/// if there is, put it into the buffer.
		/// if there is not, flush the buffer and put the input data into the buffer.
		/// if the input data is larger than the buffer itself, flush the buffer and the data.
		/// 
		/// when the buffer is flushed, the buffer is simply printed out to the terminal.
		/// 
		class TermRendererBuffer;

		#define AR_INT_FUNC_HELPER(...) __VA_ARGS__
		// TODO: make a macro define and macro undef file?
		// macro for defining a "virtual" function in the compile time interface.
		// short for "ascii renderer interface function"
		#define AR_INT_FUNC_R(signature, ret_val) AR_INT_FUNC_HELPER signature { AR_ASSERT_VOLATILE(false, #signature " should not be called from the interface class!"); return AR_INT_FUNC_HELPER ret_val; }
		// TODO: is this needed?
		#define AR_INT_FUNC(signature) AR_INT_FUNC_HELPER signature { AR_ASSERT_VOLATILE(false, #signature " should not be called from the interface class!"); }

		/// @brief interface class for the TerminalRenderer.  
		/// 
		/// this class is responsible for being the interface between the Renderer class and the final terminal output string.
		/// It Allows for very simple rendering in terms of setting a specific terminal tile to a specific value, and nothing more in terms of rendering.
		/// for more advanced rendering, see the Renderer.
		/// 
		/// the rendering part uses the setState() and drawTile() / blendTile() to draw.
		/// 
		/// setState() determins the current value that will be put at the Tile, when calling drawTile() or blendTile().
		/// 
		/// in order to view a tile at a specific position, use getTile().
		/// 
		/// to output the rendered data to the terminal, use render().
		/// 
		/// 
		/// the TerminalRenderer is also responsible for controlling different properties of the terminal, such as the title, size, position and so on.
		/// 
		/// 
		/// the render() function updates the terminal properties (title, size, position ...) as well as outputing the rendered frame to the terminal.
		/// 
		/// 
		/// implementation details:
		/// in order to generate a more optimal output string, the previously rendered frame is stored.
		/// this enables the TerminalRenderer to only generate ansi codes that overwrite parts of the terminal that have changed.
		/// 
		/// However, sometimes this frame gets invalidated, like on a resize. In this case, the previous frame cannot just be cleared,
		/// as it is not guaranteed the resized terminal will have an entierly black background.
		/// Thus a different tile is created, known as an empty tile, meaning no matter what, it will be overwritten.
		/// 
		class TerminalRendererInterface
		{
		public:

			/// @brief different properties for the terminal
			struct TerminalProps
			{
				/// @brief the title of the terminal
				std::string title;
				/// @brief the size of the terminal
				TermVert size;
				/// @brief the buffer size of the TerminalRenderer
				size_t buffer_size;

				TerminalProps(const std::string& title = "Asciir App", TermVert size = { 0, 0 }, size_t buffer_size = 1024ULL * 1024ULL * 128ULL)
					: title(title), size(size), buffer_size(buffer_size)
				{}
			};

			/// @brief structure containing info about a newly rendered terminal
			struct TRUpdateInfo
			{
				bool new_size = false;
				bool new_pos = false;
				bool new_name = false;
				bool new_zoom = false; // TODO: implement this
			};

			/// @brief structure containing two tiles, the current tile, and the previously rendered tile
			struct DrawTile
			{
				Tile current;
				Tile last = Tile::emptyTile();
			};

		public:

			TerminalRendererInterface() = default;
			~TerminalRendererInterface();

			/// @brief set the foregound colour of the currently active state tile
			void setColour(Colour colour);
			/// @brief set the background colour of the currently active state tile
			void setBackgroundColour(Colour colour);
			/// @brief set the symbol of the currently active state tile
			void setSymbol(char symbol);

			// TODO: should these be removed?
			void drawVertices(const TermVerts& vertices, DrawMode mode = DrawMode::Line);
			void drawLine(const TermVert& a, const TermVert& b);

			/// @brief fill the active frame with the passed clear tile.
			/// @note the alpha value is ignored in the clear_tile, so a completly transparent red tile, will still result in a red background.
			void clearTerminal(Tile clear_tile = Tile());
			/// @brief clear the previously rendered frame memory.
			void clearRenderTiles();

			/// @brief set the value of the state tile.
			void setState(Tile tile);
			/// @brief const version of getState()
			Tile getState() const;
			/// @brief get the current tile state
			Tile& getState();
			/// @brief replaces the tile at pos with the current tile state
			void drawTile(TInt x, TInt y);
			/// @see drawTile(TInt, TInt)
			void drawTile(const TermVert& pos) { drawTile(pos.x, pos.y); }
			/// @brief replaces the tile at pos with the specified tile. does not modify the current tile state.
			void drawTile(TInt x, TInt y, const Tile& tile);
			/// @brief  @see drawTile(TInt, TInt)
			void drawTile(const TermVert& pos, const Tile& tile) { drawTile(pos.x, pos.y, tile); }
			/// @brief blends the current tile state into the stored tile with the stored tile as the background
			void blendTile(TInt x, TInt y);
			void blendTile(const TermVert& pos) { blendTile(pos.x, pos.y); }
			/// @brief blends the tile at pos with the specified tile. does not modify the current tile state.
			void blendTile(TInt x, TInt y, const Tile& tile);
			void blendTile(const TermVert& pos, const Tile& tile) { blendTile(pos.x, pos.y, tile); }
			/// @brief retrieve the DrawTile at the passed position
			/// @return the current and previosly rendered tile at the specified position
			DrawTile& getTile(TInt x, TInt y);
			DrawTile& getTile(const TermVert& pos) { return getTile(pos.x, pos.y); }
			/// @brief set the title of the terminal
			/// @note the title will only be changed on the terminal once it has been rendered
			void setTitle(const std::string& title);
			/// @brief get the title of the terminal  
			/// this function returns the value of the latest call to setTitle(), and not the actual terminal titile
			std::string getTitle() const;

			/// @brief get the AsciiAttr instance for the current terminal
			const AsciiAttr& getAttrHandler();


			/// @brief updates the terminal properties.  
			/// this function updates the size, title, and fontsize (TODO: implement font size)
			/// @return returns a TRUpdateInfo structure containing information about the changes from the previous terminal properties to the current terminal properties.
			TRUpdateInfo update();
			/// @brief draws the currently stored frame into the terminal, and stores this frame as the previous frame.
			void draw();
			/// @brief calls update() and draw().
			TRUpdateInfo render();
			
			/// @brief request the terminal to resize to the specified size
			/// the terminal will actually resize only once update() has been called.
			void resize(TermVert size);

			/// @brief gets called when the terminal should be resized.
			/// the new size should be drawWidth() X drawHeight()
			/// it is ipmlementation specific wether the terminal resize happens at the function call, or at the next update.
			AR_INT_FUNC_R((bool) resizeBuff(), (false))

			/// @brief retrieves the number of monitors connected to the computer.
			AR_INT_FUNC_R((size_t) monitorCount() const, (0))
		
			/// @brief retrieves the resolution of the n'th monitor to the computer (in pixels).
			AR_INT_FUNC_R((TermVert) monitorSize(size_t) const, ({-1, -1}))

			/// @brief retrieves the resolution of the monitor the terminal is currently on (in pixels).
			AR_INT_FUNC_R((TermVert) monitorSize() const, ({ -1, -1 }))

			/// @brief returns the size of the terminal.
			/// @note this is the current actual size of the terminal, for the draw size, use drawSize().
			AR_INT_FUNC_R((TermVert) termSize() const, ({}))
			/// @brief get the size where the TerminalRenderer is able to draw.
			Size2D drawSize() const;
			size_t drawWidth() const { return m_tiles.width(); }
			size_t drawHeight() const { return m_tiles.height(); }
			/// @brief returns the maximum possible size of the terminal. (-1, -1) = no limit.
			AR_INT_FUNC_R((TermVert) maxSize() const, ({}))

			/// @brief returns the current position of the terminal.
			AR_INT_FUNC_R((Coord) pos() const, ({}))
			/// @brief returns the position of the terminal at the previous call to update() or render().
			Coord lastPos() { return m_pos; }
			
			/// @brief gets the current terminal font.
			/// @return a pair structure containing the name and the size of the font.
			AR_INT_FUNC_R((std::pair<std::string, Size2D>) getFont() const, ({ "", Size2D() }))

			/// @brief sets the current font of the terminal.
			/// @param the name of the font.
			/// @param new_size the font size.
			/// @return wether the function was successfull in setting the font. this will fail if the font_name is invalid (does not exist), or the size is invalid.
			// ignore unused variable warning
			#pragma warning(push)
			#pragma warning(suppress: 4100)
			AR_INT_FUNC_R((bool) setFont(const std::string& font_name, Size2D new_size), ({}))
			#pragma warning(pop)

			/// @brief resturns wether the terminal window is currently focused
			AR_INT_FUNC_R((bool) isFocused() const, ({}))

			/// TODO: should these still be here? 
			void pushBuffer(const std::string& data);
			void pushBuffer(const char* c_str);
			void pushBuffer(const char* c_buff, size_t buff_len);
			void pushBuffer(char c);

			/// @brief is called when the draw function finishes and the buffer should display its contents to the console.
			AR_INT_FUNC_R((void) flushBuffer(),());

			/// @brief get the current buffer of the TerminalRenderer buffer.
			// this should return the platform implementation of the TermRendererBuffer.
			AR_INT_FUNC_R((std::streambuf*) getBuffer(), (nullptr))
			/// @brief get the ostream for the TerminalRenderer buffer
			std::ostream& getStream(){ return m_buff_stream; }

			/// @brief return array of attributes for the current state.
			std::array<bool, ATTR_COUNT>& attributes();

		protected:
			/// @brief sets up the terminal and TerminalRenderer with the specified terminal properties
			TerminalRendererInterface(const TerminalProps& term_props);

			/// @brief initializes the renderer by loading the terminal properties passed to it as a parameter.
			/// this function should be used by the interface implementations when the terminal is ready to accept ansi codes,
			/// as this function makes use of the ansi resize and rename escape sequance
			void initRenderer(const TerminalProps& term_props);

		protected:
			arMatrix<DrawTile, Eigen::RowMajor> m_tiles;
			Coord m_pos;
			Size2D m_font_size;
			// stores the size the terminal should be resized to. If it is (0, 0), then the terminal should not be resized.
			Size2D m_term_resize;
			// this is needed, as both buffers needs to be resized on a request, and this keeps track of which buffer already has been resized.
			bool m_resize_first = false;
			Tile m_tile_state = Tile();
			std::string m_title;

			ETH::LThread m_print_thrd;

			Ref<AsciiAttr> m_attr_handler;
			std::ostream m_buff_stream;
			bool m_should_resize = false;
			bool m_should_rename = true;
		};

		#undef AR_INT_FUNC
	} // TRInterface
} // Asciir

#ifdef AR_WIN
#include "Asciir/Platform/Windows/WinTermRenderer.h"
#elif defined(AR_MAC)
#error mac is not yet supported
#elif defined(AR_LINUX)
#error linux is not yet supported
#else
#error unknown platform, cannot decide on TerminalRenderer implementation
#endif
