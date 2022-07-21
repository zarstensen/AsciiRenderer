#pragma once

#include "ECS.h"

#include "Asciir/Rendering/Shader.h"
#include "Asciir/Audio/Audio.h"
#include "Asciir/Core/Timing.h"

namespace Asciir
{
	/// @brief stores an entities parent and children.
	/// every entity should have a NodeC component on it.
	struct NodeC : public Component
	{

		// root by default
		/// @brief which entity is the parent of the current entity.
		/// if parent = INVALID_ENTITY_UID, then the current entity should be the root.
		UID parent = INVALID_ENTITY_UID;

		/// @brief which entities have the current entity as their parent.
		std::map<std::string, UID> children;

		/// @brief the name of the entity, this can be used t
		std::string name;
	};

	struct TextureC : public Component
	{
		bool enabled = true;
		Ref<Shader2D> texture;
	};

	struct PositionC : public Component
	{
		Coord position;
	};

	struct MeshC : public Component
	{
		Tile tile;
		Mesh mesh;
	};

	struct TransformC : public Component
	{
		Transform transform;
	};

	struct AudioC : public Component
	{
		AudioPlayer player;
		AudioSource source;
	};

	class NativeScriptSystem;

	struct NativeScript : public Component
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

	struct NativeScriptC : public Component
	{
		Ref<NativeScript> script;
	};

	struct LuaScriptC : public Component
	{
		bool enable = true;

		Path script;
	};

}
