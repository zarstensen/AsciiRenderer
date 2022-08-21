#include "ECS.h"

namespace Asciir
{
	// ============ EntityBlueprint ============
	
	template<typename ...Args>
	template<typename ...InitArgs, std::enable_if_t<sizeof...(InitArgs) <= sizeof...(Args), bool>>
	EntityBlueprint<Args...>::EntityBlueprint(InitArgs ... init_args)
	{
		// call recursive helper function to fill the tuple
		// Start Index v
		if constexpr(sizeof...(InitArgs) > 0)
			fillInitValues<0, InitArgs...>(init_args...);
	}

	template<typename ...Args>
	template<size_t index, typename InitArg, typename ... Rest,
		std::enable_if_t<std::is_same_v<InitArg, std::tuple_element_t<index, std::tuple<Args...>>>, bool>>
		void EntityBlueprint<Args...>::fillInitValues(InitArg arg, Rest ... rest)
	{
		std::get<index>(m_init_values) = arg;

		// recursivly fill the rest of the tuple
		fillInitValues<index + 1, Rest...>(rest...);
	}

	template<typename ...Args>
	template<size_t index, typename InitArg, 
		std::enable_if_t<std::is_same_v<InitArg, std::tuple_element_t<index, std::tuple<Args...>>>, bool>>
	void EntityBlueprint<Args...>::fillInitValues(InitArg arg)
	{
		std::get<index>(m_init_values) = arg;
	}

	// ============ Scene ============

	template<typename TBlueprint, std::enable_if_t<enable_if_same_template<TBlueprint, EntityBlueprint>::value, bool>>
	Entity Scene::createEntity(const TBlueprint& entity_blueprint)
	{
		Entity new_entity = createEntity();

		if constexpr (std::tuple_size_v<TBlueprint::CompList> > 0)
		{
			const typename TBlueprint::CompList& component_list = entity_blueprint.getComponentValues();
			readBlueprint<0>(new_entity, component_list);
		}

		return new_entity;
	}

	template<size_t index, typename TTuple>
	void Scene::readBlueprint(Entity entity, const TTuple& blueprint_components)
	{
		entity.add<std::tuple_element_t<index, TTuple>>(std::get<index>(blueprint_components));

		if constexpr (index + 1 < std::tuple_size_v<TTuple>)
			readBlueprint<index + 1>(entity, blueprint_components);
	}
}
