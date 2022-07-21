#include "Systems.h"

#include "Asciir/Rendering/Renderer.h"

namespace Asciir
{
	void ShaderRenderSystem::process(Scene&)
	{
		if (getComponent<TextureC>().enabled)
			Renderer::submit(getComponent<TextureC>().texture, getComponent<TransformC>().transform);
	}

	void MeshRenderSystem::process(Scene&)
	{
		Renderer::submit(getComponent<MeshC>().mesh, getComponent<MeshC>().tile, getComponent<TransformC>().transform);
	}

	void NativeScriptSystem::process(Scene& scene)
	{
		// if the script comonent has just been enabled / disabled, call the add / remove method.

		Ref<NativeScript> script = getComponent<NativeScriptC>().script;

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
			script->process(dt);
	}
}