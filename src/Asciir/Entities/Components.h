#pragma once

#include "ECS.h"

#include "Asciir/Rendering/Shader.h"
#include "Asciir/Audio/Audio.h"
#include "Asciir/Core/Timing.h"

namespace Asciir
{
	/// @brief stores an entities parent and children.
	/// every entity should have a NodeC component on it.
	struct Node
	{

		// root by default
		/// @brief which entity is the parent of the current entity.
		/// if parent = INVALID_ENTITY_UID, then the current entity should be the root.
		Entity parent;

		/// @brief which entities have the current entity as their parent.
		std::map<std::string, Entity> children;

		/// @brief the name of the entity, this can be used t
		std::string name;
	};

	struct TextureC
	{
		bool enabled = true;
		Ref<Shader2D> texture;
	};

	struct MeshC
	{
		Tile tile;
		Mesh mesh;
	};

	struct AudioC
	{
		AudioPlayer player;
		AudioSource source;
	};

	class NativeScriptSystem;

	struct NativeScript
	{
		bool enable = true;
		Entity entity;

		virtual void process(DeltaTime) {}
		
		virtual void add() {}

		virtual void remove() {}

	private:
		bool m_added = false;

		friend NativeScriptSystem;
	};

	struct LuaScript
	{
		bool enable = true;

		Path script;
	};

}
