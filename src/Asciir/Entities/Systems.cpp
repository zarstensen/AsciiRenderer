#include "Systems.h"

#include "Asciir/Rendering/Renderer.h"

namespace Asciir
{
	//void ShaderRenderSystem::process(Scene&)
	//{
	//	if (getComponent<TextureC>().enabled)
	//		Renderer::submit(getComponent<TextureC>().texture, getComponent<TransformC>().transform);
	//}

	//void MeshRenderSystem::process(Scene&)
	//{
	//	Renderer::submit(getComponent<MeshC>().mesh, getComponent<MeshC>().tile, getComponent<TransformC>().transform);
	//}

	//void NativeScriptSystem::process(Scene& scene)
	//{
		
	void ShaderRendererSystem(System*, Scene& scene)
	{
		for (auto [_, texture, transform] : scene.getReg().view<TextureC, Transform>().each())
		{
			if (texture.enabled)
				Renderer::submit(texture.texture, transform);
		}
	}

	void MeshRendererSystem(System*, Scene& scene)
	{
		for (auto [_, mesh, transform] : scene.getReg().view<MeshC, Transform>().each())
		{
			Renderer::submit(mesh.mesh, mesh.tile, transform);
		}
	}

	//}
}