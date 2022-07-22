#pragma once

#include "ECS.h"

#include "Components.h"

namespace Asciir
{
	void ShaderRendererSystem(System*, Scene& scene);

	void MeshRendererSystem(System*, Scene& scene);

	class NativeScriptSystem : public System
	{
	public:
		NativeScriptSystem()
			: System((SystemPtr)&NativeScriptSystem::callScripts) {}

		static void callScripts(NativeScriptSystem* system, Scene& scene)
		{
			DeltaTime curr_time = getTime();
			system->dt = curr_time - system->prev_process;

			system->prev_process = curr_time;

			for (auto [_, script] : scene.getReg().view<Ref<NativeScript>>().each())
			{
				// if the script comonent has just been enabled / disabled, call the add / remove method.

				if (script->enable == true && script->m_added == false)
				{
					script->m_added = true;
					script->add();
				}
				else if (script->enable == false && script->m_added == true)
				{
					script->m_added = false;
					script->remove();
				}

				if (script->enable)
					script->process(system->dt);
			}
		}

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
		}
	};

}
