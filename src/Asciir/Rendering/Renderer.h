#pragma once

#include "Mesh.h"
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
			/// @brief a referene to the mesh that should be rendered
			Ref<Mesh> mesh;
			/// @brief the tile the rendered mesh should be drawn with
			Tile tile;
			/// @brief the transformation the mesh should have.
			Transform transform;
		};

		/// @brief structure containing information for rendering a Shader2D  
		/// @note this structure should only be instantiated by the Renderer itself, and a workflow where this is instantiated manually should be avoided
		struct ShaderData
		{
			/// @brief a reference to the shader that should be rendered
			Ref<Shader2D> shader;
			/// @brief the transform the shader should have
			Transform transform;
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

		// submit functions
		/// @brief submits the given mesh data to the render queue
		// TODO: should this be a reference? mesh might be modified whilst the renderer is rendering.
		static void submitMesh(Ref<Mesh> mesh, Tile tile, Transform transform = NoTransform);
		/// @brief submits the given shader to the render queue 
		static void submitShader(Ref<Shader2D> shader, Transform transform = NoTransform);
		/// @brief submits the given tile to the render queue
		static void submitTile(TermVert pos, Tile tile);
		static void submitToQueue(QueueElem new_elem);
		static Ref<Mesh> submitRect(s_Coords<2> verts, Tile tile);
		static Tile viewTile(TermVert pos);
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
		/// @brief retrieves the current size of the terminal, for the previously rendered fram size, see lastSize()
		static Size2D size();

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

		/// @brief render the given mesh data
		static void drawMeshData(MeshData& data);
		/// @brief render the given shader data
		static void drawShaderData(ShaderData& data, const DeltaTime& time_since_start, size_t frames_since_start);
		/// @brief render the given tile data
		static void drawTileData(TileData& data);
		/// @brief render the given clear data
		static void drawClearData(ClearData& data);

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
		/// @brief 
		static arMatrix<Tile> s_visible_terminal;

		/// @brief the app will wait until the minimum delta time is hit, after each update
		/// @brief DEFAULT: no limit
		static DeltaTime s_min_dt;
	};
}

#include "Renderer.ipp"
