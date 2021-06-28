#pragma once

#include "Mesh.h"
#include "TerminalRenderer.h"
#include "Asciir/Math/Vertices.h"
#include "Asciir/Core/Engine.h"

namespace Asciir
{
	class Renderer
	{
		friend AREngine;
	public:

		struct QueueElem
		{
			Ref<Mesh> mesh;
			Tile tile;
			Transform transform;
		};

		static void init();

		// draw functions
		static void submitToQueue(Ref<Mesh> mesh, Tile tile, Transform transform = NoTransform);
		static Ref<Mesh> submitRect(s_Coords<2> verts, Tile tile);
		static void drawTile(Coord pos, Tile tile);
		static Tile viewTile(Coord pos);

		// environment functions

		// terminal functions
		static void clear(Tile tile = Tile(BLACK8, WHITE8, ' '));
		static void resize(Size2D size);
		static Size2D size();

	protected:
		
		template<typename T, std::enable_if_t<is_vertices_vtype_v<Coord, T>, bool> = false>
		static Coords projectCoordsToTerminal(const T& coords);

		static void flushQueue();
		static void drawQueueElem(const QueueElem& elem);

		
		
		static TerminalRenderer* s_renderer;
		static const AsciiAttr* s_attr_handler;
		static std::vector<QueueElem> s_render_queue;
	};

	

}

#include "Renderer.ipp"
