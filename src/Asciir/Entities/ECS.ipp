#include "ECS.h"

namespace Asciir
{
	/*
	* EntityBlueprint definitions
	*/
	
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

	/*
	* System definitions
	*/
	template<typename First, typename ...Args, std::enable_if_t<(sizeof...(Args) > 0), bool>>
	void System::requireComponents()
	{
		m_system_components.reserve(m_system_components.size() + sizeof...(Args) + 1);

		requireComponents<First>();
		requireComponentsHelper<Args...>();
	}

	template<typename TComp, enable_if_component<TComp>>
	void System::requireComponents()
	{
		AR_ASSERT_MSG(m_system_components.find(typeid(TComp)) == m_system_components.end(), "requireComponents was called with a component already required: ", typeid(TComp).name());
		m_system_components.insert(typeid(TComp));
	}

	template<typename First, typename ... Args, std::enable_if_t<(sizeof...(Args) > 0), bool>>
	void System::requireComponentsHelper()
	{
		// does not reserve

		requireComponents<First>();
		requireComponentsHelper<Args...>();
	}

	template<typename First, typename ...TComps>
	void System::unrequireComponents()
	{
		unrequireComponents<First>();
		unrequireComponents<TComps...>();
	}

	template<typename TComp, enable_if_component<TComp>>
	void System::unrequireComponents()
	{
		AR_ASSERT_MSG(m_system_components.find(typeid(TComp)) != m_system_components.end(), "unrequireComponents was called with a component not required: ", typeid(TComp).name());

		m_system_components.erase(typeid(TComp));
	}


	template<typename TEvent, enable_if_event<TEvent>>
	void System::createEvent(TEvent s_event)
	{
		s_event.m_components = m_active_components;
		s_event.m_scene = m_active_scene;

		m_active_scene->dispatchEvent(s_event);
	}
	
	template<typename TEvent, enable_if_event<TEvent>>
	void System::subscribe()
	{
		// update scenes
		for (Scene* scene : m_system_scenes)
			scene->subscribeSystem<TEvent>(*this);
		
		m_subscribed_events.insert(typeid(TEvent));
	}

	template<typename TEvent, enable_if_event<TEvent>>
	void System::unsubscribe()
	{
		// update scenes
		for (Scene* scene : m_system_scenes)
			scene->unsubscribeSystem<TEvent>(*this);

		m_subscribed_events.erase(typeid(TEvent));
	}


    /*
    * SceneView and ComponentIterator definitions
    */

	template<typename ...TComps>
	SceneView::SceneView(Scene& target_scene)
		: SceneView(target_scene)
	{
		std::array<std::type_index, sizeof...(TComps)> type_arr = { typeid(TComps)... };

		for (std::type_index type_id : type_arr)
			addComponent(type_id);

		prepare();
	}

	/*
	* Scene definitions
	*/

	template<typename TBlueprint, std::enable_if_t<enable_if_same_template<TBlueprint, EntityBlueprint>::value, bool>>
	UID Scene::createEntity(const TBlueprint& entity_blueprint)
	{
		UID new_entity = createEntity();

		if constexpr (std::tuple_size_v<TBlueprint::CompList> > 0)
		{
			const TBlueprint::CompList& component_list = entity_blueprint.getComponentValues();
			readBlueprint<0>(new_entity, component_list);
		}

		return new_entity;
	}

	template<typename TComp, enable_if_component<TComp>>
	void Scene::addComponents(UID entity_id, TComp component_data)
	{
		AR_ASSERT_MSG(hasEntity(entity_id), "cannot add a component to an entity not in the scene\nEntity UID: ", entity_id);

		// create a component buffer for the passed type if it does not exist
		if (m_components.find(typeid(TComp)) == m_components.end())
		{
			m_components[typeid(TComp)] = ComponentBuffer(sizeof(TComp));
			m_components[typeid(TComp)].setSize(m_entity_count);
		}

		m_components[typeid(TComp)].set(entity_id, component_data);

		// set the newly added components entity uid
		getComponents<TComp>(entity_id).m_entity_id = entity_id;
	}
	
	template<typename TComp, typename ...Args, std::enable_if_t<(sizeof...(Args) > 0), bool>>
	void Scene::addComponents(UID entity_id)
	{
		addComponents<TComp>(entity_id);
		addComponents<Args...>(entity_id);
	}

	template<typename TComp, typename ...Args, std::enable_if_t<(sizeof...(Args) > 0), bool>>
	void Scene::addComponents(UID entity_id, TComp component_data, Args ...args)
	{
		addComponents<TComp>(entity_id, component_data);
		addComponents<Args...>(entity_id, args...);
	}

	template<typename TComp, enable_if_component<TComp>>
	void Scene::removeComponents(UID entity_id)
	{
		AR_ASSERT_MSG(hasEntity(entity_id), "cannot remove a component from an entity not in the scene\nEntity UID: ", entity_id);

		AR_ASSERT_MSG(m_components.find(typeid(TComp)) != m_components.end(), "cannot remove component if no ComponentBuffer of the component type has been created\nEntity UID: ", entity_id);

		AR_ASSERT_MSG(m_components[typeid(TComp)].hasIndex(entity_id), "cannot remove component if it has not been added\nEntity UID: ", entity_id);

		m_components[typeid(TComp)].remove(entity_id);
	}

	template<typename TComp, typename ...Args, std::enable_if_t<(sizeof...(Args) > 0), bool>>
	void Scene::removeComponents(UID entity_id)
	{
		removeComponents<TComp>(entity_id);
		removeComponents<Args...>(entity_id);
	}

	template<typename TComp, enable_if_component<TComp>>
	TComp& Scene::getComponents(UID entity_id)
	{
		AR_ASSERT_MSG(hasEntity(entity_id), "cannot get a component for an entity not in the scene\nEntity UID: ", entity_id);

		AR_ASSERT_MSG(m_components.find(typeid(TComp)) != m_components.end(), "cannot get component if no ComponentBuffer of the component type has been created\nEntity UID: ", entity_id);

		AR_ASSERT_MSG(m_components[typeid(TComp)].hasIndex(entity_id), "cannot get component if it has not been added\nEntity UID: ", entity_id);

		return *(TComp*) m_components[typeid(TComp)].get(entity_id);
	}

	template<typename TComp, enable_if_component<TComp>>
	TComp& Scene::getComponentIndexed(size_t index)
	{
		return getCOmponentIndexed(typeid(TComp), index);
	}

	template<typename ...Args, std::enable_if_t<(sizeof...(Args) > 1), bool>>
	std::tuple<Args&...> Scene::getComponents(UID entity_id)
	{
		return { getComponents<Args>(entity_id)... };
	}
	
	template<typename TComp, enable_if_component<TComp>>
	size_t Scene::componentCount()
	{
		return componentCount(typeid(TComp));
	}

	template<typename TEvent, enable_if_event<TEvent>>
	void Scene::dispatchEvent(TEvent& s_event)
	{
		for (System* subscribed_system : m_event_subscribtions[typeid(TEvent)])
		{
			subscribed_system->onEventRecieve(s_event);
		}
	}

	template<size_t index, typename TTuple>
	void Scene::readBlueprint(UID entity_id, const TTuple& blueprint_components)
	{
		addComponents<std::tuple_element_t<index, TTuple>>(entity_id, std::get<index>(blueprint_components));

		if constexpr (index + 1 < std::tuple_size_v<TTuple>)
			readBlueprint<index + 1>(entity_id, blueprint_components);
	}
}
