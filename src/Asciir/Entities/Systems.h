#pragma once

#include "ECS.h"

#include "Components.h"

namespace Asciir
{
	class ShaderRenderSystem : public System
	{
	public:
		/// @brief sets up the system, in this case, it specifies the required components for the rendersystem.
		ShaderRenderSystem()
		{
			requireComponents<TextureC, TransformC>();
		}

		/// @brief submits the texture and transform copmonent the the Render queue, if the texture is enabled.
		virtual void process(Scene& scene) override;
	};

	class MeshRenderSystem : public System
	{
	public:
		MeshRenderSystem()
		{
			requireComponents<MeshC, TransformC>();
		}

		virtual void process(Scene& scene) override;
	};

	class NativeScriptSystem : public System
	{
	public:
		NativeScriptSystem()
		{
			requireComponents<NativeScriptC>();
		}

		virtual void onStart()
		{
			DeltaTime curr_time = getTime();
			dt = curr_time - prev_process;

			prev_process = curr_time;
		}

		// Inherited via System
		virtual void process(Scene& scene) override;

	protected:
		// the deltatime to pass to the scripts
		DeltaTime dt;
		// the timestamp of the last process call
		DeltaTime prev_process = getTime();
	};

	class LuaScriptSystem : public System
	{
	public:
		LuaScriptSystem()
		{
			requireComponents<LuaScriptC>();
		}
	};

}
