#pragma once

#include "Mesh.h"
#include "TerminalRenderer.h"
#include "Texture.h"

#include "Asciir/Maths/Vertices.h"
#include "Asciir/Core/Application.h"

namespace Asciir
{
	class Renderer
	{
		friend ARApp;
	public:
		struct MeshData
		{
			Ref<Mesh> mesh;
			Tile tile;
			Transform transform;
		};

		struct ShaderData
		{
			Ref<Shader2D> shader;
			Transform transform;
		};

		struct TileData
		{
			TermVert pos;
			Tile tile;
		};

		typedef Tile ClearData;

		// mesh data, texture data, point data or clear data
		typedef std::variant <MeshData, ShaderData, TileData, ClearData> QueueElem;

		static void init();

		// submit functions
		static void submitMesh(Ref<Mesh> mesh, Tile tile, Transform transform = NoTransform);
		static void submitShader(Ref<Shader2D> shader, Transform transform = NoTransform);
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

		// set the minimum delta time between updates
		static void setMinDT(DeltaTime min_dt) { AR_ASSERT(min_dt.milliSeconds() >= 0); s_min_dt = min_dt; }

		static DeltaTime getMinDT() { return s_min_dt; }

		// terminal functions
		static void clear(Tile tile = Tile(BLACK8, WHITE8, ' '));
		static void resize(Size2D size);
		static Size2D size();

	protected:

		template<typename T, std::enable_if_t<is_vertices_vtype_v<Coord, T>, bool> = false>
		static Coords projectCoordsToTerminal(const T& coords);

		// swaps and reallocates queues if necesary
		static void swapQueues();
		static void flushRenderQueue(const DeltaTime& time_since_start, const size_t& frames_since_start);

		static void drawMeshData(const MeshData& data);
		static void drawShaderData(const ShaderData& data, const DeltaTime& time_since_start, const size_t& frames_since_start);
		static void drawTileData(const TileData& data);
		static void drawClearData(const ClearData& data);

		static void waitMinDT(DeltaTime curr_dt);

		static TerminalRenderer* s_renderer;
		static const AsciiAttr* s_attr_handler;
		static std::vector<QueueElem>* s_submit_queue;
		static std::vector<QueueElem>* s_render_queue;
		/// @brief 
		static arMatrix<Tile> s_visible_terminal;

		// the app will wait until the minimum delta time is hit, after each update
		// DEFAULT: no limit
		static DeltaTime s_min_dt;
	};
}

#include "Renderer.ipp"
