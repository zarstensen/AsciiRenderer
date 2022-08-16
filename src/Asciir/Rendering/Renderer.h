#pragma once

#include "Mesh.h"
#include "Primitives.h"
#include "TerminalRenderer.h"
#include "Texture.h"

#include "Asciir/Maths/Vertices.h"
#include "Asciir/Core/Application.h"

namespace Asciir
{
	/// @brief the static class responsible for recieving renderable data structures, managing them, rendering them and supplying the rendered data to the TerminalRenderer instance.
	/// 
	/// list of renderable datastructures are:
	/// Tile
	/// Shader2D
	/// Mesh
	///	
	/// these datastructures can be submitted through various submit functions
	/// @see submitMesh(), submitShader(), submitTile(), submitToQueue()
	///
	/// the datastructures will not be rendered as soon as they are submitted, but only once onUpdate() has returned.
	///
	/// the last rendered frame can be accessed through viewTile().
	///
	///	implementation details:
	/// 
	/// when data is submitted to the renderer, it is pushed into a *render queue* which will be processed at the end of the current application update.
	/// once this point is reached, this queue will be swapped with another queue, which has just been processed.
	/// This new queue will now be the queue which will recieve any newly submitted data.  
	/// 
	/// It is implemented as such in order to allow for rendering and running the application loop at the same time, as this approach enables the application to modify the render queue whilst the renderer is rendering.
	/// 
	/// The render queue are not reallocated at every submit, as AR_RENDER_QUEUE_MARGIN defines how many render objects needs to be added / removed, before the capacity is changed.
	/// 
	/// The last rendered frame is stored seperatly from the TerminalRenderer,
	/// instead of using the already supplied previous rendererd frame, as this frame is not guaranteed to have the correct information.
	///
	/// for example: if the terminal is resized, the last frame will be cleared, whereas the Renderer last frame will be preserved.
	/// 
	/// This also means any functions that reads tiles, is reading from the previously rendered frame instead of the current one.
	///
	class Renderer
	{
		friend ARApp;
	public:

		/// @brief structure containing information for rendering a Mesh instance  
		/// @note this structure should only be instantiated by the Renderer itself, and a workflow where this is instantiated manually should be avoided
		struct MeshData
		{
			/// @brief the mesh that should be rendered
			Mesh mesh;
			/// @brief the tile the rendered mesh should be drawn with
			Tile tile;
			/// @brief quad descibing an area which contains the entire mesh, taking into account the transformation (should be as small as possible)
			Quad visible;
		};

		/// @brief structure containing information for rendering a Shader2D  
		/// @note this structure should only be instantiated by the Renderer itself, and a workflow where this is instantiated manually should be avoided
		struct ShaderData
		{
			/// @brief the shader that should be rendered
			/// @note even though this is a reference, it does not store a reference to the submitted texture. It is only a reference, as Shader2D is an abstract class.
			Ref<Shader2D> shader;
			/// @brief the transform the shader should have
			Transform transform;
			/// @brief quad descibing an area which contains the entire shader, taking into account the transformation (should be as small as possible)
			/// if shader has no size, this will be set to (-1, -1) (-1, -1) and should be ignored.
			Quad visible = Quad({ -1, -1 }, {-1, -1});
		};

		/// @brief structure containing information for rendering a single pixel / tile on the terminal
		/// @note this structure should only be instantiated by the Renderer itself, and a workflow where this is instantiated manually should be avoided
		struct TileData
		{
			/// @brief the tile that should be rendered
			Tile tile;
			/// @brief the position, in the terminal, of the tile
			TermVert pos;
		};

		typedef Tile ClearData;

		/// @brief the datatype used in the render queue
		/// mesh data, texture data, point data or clear data
		typedef std::variant<MeshData, ShaderData, TileData, ClearData> QueueElem;

		/// @brief initialize the renderer.
		/// setsup all the static references that have been setup before the renderer.
		static void init();

		/// @brief defaults to number of avaliable threads on the system. @see setThreads()
		static void setThreads() { setThreads(std::thread::hardware_concurrency()); }
		/// @brief sets the number of threads to be used when rendering the frame.
		/// @note everytime this function is called, all the threads are reallocated and restarted, so call this function as little as possible, preferably only once at the application start.
		static void setThreads(uint32_t thread_count);
		/// @brief returns the number of threads used when rendering a frame.
		static uint32_t getThreads() { return (uint32_t) m_render_thread_pool.size(); }

		/// @brief the number of tiles to be processed by a thread, at a time.  
		/// 
		/// example:
		/// a frame with the dimenstions 10 x 10 has 5 threads, and the thread length is 10.  
		/// In this case, each thread will render a column at a time, before moving along to the next avaliable tiles.
		/// 
		static inline uint32_t thrd_tile_count = 256;

		// submit functions
		/// @brief submits the given mesh data to the render queue
		// TODO: should this be a reference? mesh might be modified whilst the renderer is rendering.
		static void submit(const Mesh& mesh, Tile tile, Transform transform = NoTransform);
		/// @brief submits the given shader to the render queue 
		template<typename TShader, std::enable_if_t<std::is_base_of_v<Shader2D, TShader>, bool> = false>
		static void submit(Ref<TShader> shader, Transform transform = NoTransform);
		/// @brief submits the given tile to the render queue
		static void submit(TermVert pos, Tile tile);
		static void submitToQueue(QueueElem new_elem);
		static void submitRect(s_Coords<2> verts, Tile tile);
		static Tile viewTile(TermVert pos);

		/// @brief sets the title of the terminal
		static void setTitle(const std::string& title) { s_renderer->setTitle(title); }

		/// @see setFontName() and setFontSize()
		static bool setFont(const std::string& font_name, Size2D new_size) { return s_renderer->setFont(font_name, new_size); }

		/// @see getFontName() and getFontSize()
		/// @return a pair containing the name and size of the font.
		static std::pair<std::string, Size2D> getFont() { return s_renderer->getFont(); }

		/// @brief chanes the current font of the terminal
		/// @note depending on the platform, this effect might take effect instantly *(Windows)* or on the Application update.
		/// @param font_name the name of the font. Must be a font accesible to the terminal, meaning only default terminal fonts are avaliable, unless the terminal prefrences are modified, to include a custom font.
		static bool setFontName(const std::string& font_name) { return s_renderer->setFont(font_name, s_renderer->getFont().second); }

		/// @brief retrieves the name of the current font.
		static std::string getFontName() { return getFont().first; }

		/// @brief sets the font size
		/// @attention the new font size is not guaranteed to match the passed size, as not all sizes are supported. Instead, the closest approximation will be used.
		/// @param size the width and height of the font
		static bool setFontSize(Size2D size) { return s_renderer->setFont(s_renderer->getFont().first, size); }

		/// @brief retrieves the size of the current font.
		/// @return a Size2D structure containing the width and height, of the font, in pixels.
		// TODO: a string is allocated here when it is not needed.
		static Size2D getFontSize() { return getFont().second; }

		/// @brief sets the current resolution of the terminal (in PIXELS not CELLS), by changing the font size.
		/// the final resolution will always be the closest value to res, whilst being smaller than res
		/// and larger than the terminal width and height (in CELLS).
		/// 
		/// example:
		///		if the terminal is currently 10x10 cells, and setResolution({30, 30}) is called
		///		the font size will be set to (3, 3), and the resolution of the terminal will be (30, 30) pixels.
		///		
		///		if setResolution({31, 31}) is called, the resolution will still be (30, 30) pixels,
		///		as the terminal resolution cannot get closer to the passed resolution without changing the CELL count.
		/// 
		///		finally, if setResolution({1, 1}) is called, the resolution will end up being (10, 10) pixels,
		///		as the final resolution is unable to be smaller than the cell count.
		/// 
		/// @param res the resolution to approximate.
		/// 
		/// @return the final size of the terminal, after it has been fitted according to the above rules.
		static Size2D setResolution(Size2D res);

		/// @brief retrieve the current resolution (pixels) of the terminal window.
		/// 
		/// for number of cells, see size()
		/// 
		/// @return the size of the terminal window.
		static Size2D getResolution() { return size().array() * getFontSize().array(); }

		/// @brief decrease the cell size by scale whilst (attempting) to maintain the current terminal resolution.
		/// 
		/// affects both cell size and font size.
		/// 
		/// if the scale does not divide evently into the (size)[size()] of the terminal, the closest size, smaller than the wanted terminal size, is used instead.
		/// 
		/// if the return value is the same as (size() / scale, getFontSize() * scale) (inc. decimals), the resolution of the terminal will have been reserved.
		/// 
		/// @return the final terminal and font size , after it has been fitted according to the above rules.
		/// order: (term_size, font_size)
		static std::pair<Size2D, Size2D> upscale(Size2D scale);
		/// @brief increses the cell size by scale whilst (attempting) to maintain the current terminal resolution.
		/// 
		/// affects both cell size and font size.
		/// 
		/// if the scale does not divide evently into the (font size)[getFontSize()] of the terminal, the closest size,
		/// smaller than the wanted font size, is used instead.
		/// 
		/// if the return value is the same as (size() * scale, getFontSize() / scale) (inc. decimals), the resolution of the terminal will have been reserved.
		/// 
		/// @return the final terminal and font size , after it has been fitted according to the above rules.
		/// order: (term_size, font_size)
		static std::pair<Size2D, Size2D> downscale(Size2D scale);

		/// @brief grabs a section of the screen inside the rectangle described by rect_start and rect_offset
		/// @param rect_start the top right corner of the section to be captured
		/// @param rect_offset the bottom right corner of the section to be captured.
		/// {-1, -1} defaults to the lower right of the terminal
		/// @return a Shader2D containing a **COPY** of the terminal screen,
		/// meaning any changes done afterwards to the terminal, will not be reflected in the captured section
		static Texture2D grabScreen(TermVert rect_start = { 0, 0 }, TermVert rect_offset = { -1 , -1 });

		// environment functions

		/// @brief set the minimum delta time between updates.
		/// this should be used to limit the fps of the application
		static void setMinDT(DeltaTime min_dt) { AR_ASSERT(min_dt.milliSeconds() >= 0); s_min_dt = min_dt; }

		/// @brief recieve the current minimum delta time between updates.
		/// @return 
		static DeltaTime getMinDT() { return s_min_dt; }

		// terminal functions
		/// @brief clear the terminal.
		/// fills the entire terminal canvas with the passed tile.
		static void clear(Tile tile = Tile(BLACK8, WHITE8, ' '));
		/// @brief resizes the terminal to the given size.
		static void resize(Size2D size);
		/// @brief retrieves the current size of the terminal, for the previously rendered frame size, see lastSize()
		static Size2D size();
		/// @brief retrieves the maximum possible size of the terminal, for the current font configuration.
		static Size2D maxSize() { return s_renderer->maxSize(); }

		/// @brief retreives the current width of the terminal
		static TInt width();
		/// @brief retreives the current height of the terminal
		static TInt height();

		/// @brief returns the size of the previously rendered frame.
		static Size2D lastSize()
		{
			return Size2D((size_t) - 1, (size_t) - 1);
		}

	protected:

		// TODO: is this used?
		template<typename T, std::enable_if_t<is_vertices_vtype_v<Coord, T>, bool>>
		Coords projectCoordsToTerminal(const T& coords);

		/// @brief swaps and reallocates queues if necesary
		static void swapQueues();

		/// @brief renders and outputs the current render queue to the terminal.
		/// @param time_since_start the time since the start of the application
		/// @param frames_since_start the number of frames rendered up until now
		static void flushRenderQueue(const DeltaTime& time_since_start, size_t frames_since_start);

		/// @brief global delta time value for use by render threads
		/// should be set at the start of every render, so all threads have the same value
		static inline DeltaTime m_curr_dt;
		/// @brief global delta frame value for use by render threads
		static inline size_t m_curr_df;
		/// @brief single thread resbonsible for partially rendering the current frame together with other threads.  
		static void renderThrd();

		// TODO: these should be modified to return a tile, instead of rendering the entire thing.
		/// @brief render the given mesh data
		static Tile drawMeshData(MeshData& data, TInt x, TInt y);
		/// @brief render the given shader data
		static Tile drawShaderData(ShaderData& data, TInt x, TInt y, const DeltaTime& time_since_start, size_t frames_since_start);
		/// @brief render the given tile data
		static Tile drawTileData(TileData& data, TInt x, TInt y);
		/// @brief render the given clear data
		static void drawClearData(ClearData& data);

		/// @brief renders a single tile of the frame.  
		///	
		/// only renders neccesary QueueElems, meaning it skips any elements that do not contain the tile, and also skips any elements that do not have an effect on the final result.  
		/// a QueueElem is determined to have no effect, if the QueueElem above it has an alpha value of 255, and is not empty.
		/// 
		/// the QueueElems get rendered in order of last in, first out, meaning the latest submitted QueueElem will be rendered first.
		/// 
		static void drawTile(TInt y, TInt x, const DeltaTime& dt, size_t df);

		/// @brief waits until the minimum delta time is hit, assuming the passed time has already passed
		/// 
		/// simplified: this functions waits for min_dt - curr_dt.
		/// 
		/// @param curr_dt the time spent from render start until render end.
		static void waitMinDT(DeltaTime curr_dt);

		static TerminalRenderer* s_renderer;
		static const AsciiAttr* s_attr_handler;
		static std::vector<QueueElem>* s_submit_queue;
		static std::vector<QueueElem>* s_render_queue;
		static arMatrix<Tile> s_visible_terminal;

		/// @brief the app will wait until the minimum delta time is hit, after each update
		/// @brief DEFAULT: no limit
		static DeltaTime s_min_dt;


		static inline std::mutex m_mutex;
		static inline std::vector<ETH::LThread> m_render_thread_pool;
		/// @brief the start position of the next tile chunk that needs to be rendered.
		static inline std::atomic<uint32_t> m_avaliable_tile;
	};
}

#include "Renderer.ipp"
