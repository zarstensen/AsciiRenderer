#pragma once

#include "Asciir/Event/Event.h"

namespace Asciir
{

	using UID = size_t;

	static constexpr UID INVALID_ENTITY_UID = 0;

	
	// a class that can be associated with an entity and represents some data about said entity
	// should only hold data and should not be associated with any functionallity
	// acts as a base class for user and predefined components
	class Component
	{
	public:
		Component(UID uid = INVALID_ENTITY_UID)
			: m_entity_id(uid) {}

		UID getEntUID() const { return m_entity_id; }

	protected:
		UID m_entity_id;
	};


	template<typename TComp>
	using enable_if_component = enable_if_base_of<TComp, Component>;

	class System;

	class SystemEvent
	{
		friend System;
	
		UID getEntityID() { return m_entity_id; }
		template<typename TComp, enable_if_component<TComp> = false>
		Ref<TComp> getComponent();

	protected:
		SystemEvent(std::map<std::type_index, Ref<Component>>& components, UID entity);

		std::map<std::type_index, Ref<Component>>& m_components;
		UID m_entity_id;
	};

	template<typename TEvent>
	using enable_if_event = enable_if_base_of<TEvent, SystemEvent>;

	class Scene;

	// a class that reads and modifies one or more components.
	// can alert other systems via SystemEvents.
	// acts as a base class for all user and library predefined systems.
	class System
	{
	public:

		System() = default;
		virtual ~System() {}

		// user implemented function.
		// whenever a system finds an entity that contains the specified components, process is called with the entity uid and components related to the entity.
		// events should also be created inside process.
		virtual void process(const UID entity) = 0;

		// should be called by the user when the system needs to start processing entities.
		// this is not needed if the system only relies on events from other systems.
		void run();

		template<typename TComp, enable_if_component<TComp> = false>
		Ref<TComp> getComponent();

		// automaticlly creates and populates an event of specified type and dispatches it to other systems in the associated scenes.
		template<typename TEvent, enable_if_event<TEvent> = false>
		void createEvent();

		// asocciates a scene with the system
		void addScene(Ref<Scene> scene);
		// unasocciates a scene with the system
		void removeScene(Ref<Scene> scene);

		template<typename TEvent, enable_if_event<TEvent> = false>
		void subscribe();
		template<typename TEvent, enable_if_event<TEvent> = false>
		void unsubscribe();

	protected:

		// what components the system needs to function
		std::unordered_set<UID> m_system_components;
		// what scenes the system is a part of
		std::vector<Ref<Scene>> m_system_scenes;

		// stores a reference for each component currently being processed.
		std::map<std::type_index, Ref<Component>> m_active_components;
		// UID for entity currently being processed
		UID m_active_entity;
	};

	// class used by scene for storing components of same id in continous memory
	class ComponentBuffer
	{
	public:
		ComponentBuffer(size_t data_size);

		uint8_t* get(size_t indx);
		void remove(size_t indx);
		void append(uint8_t* data);

		uint8_t* operator[](size_t indx);

	protected:

		size_t m_data_size;
		std::vector<uint8_t> m_data;

	};

	// class representing a collection of entities and systems.
	// should not be inherited and should only be defined by its entities and systems attatched.
	class Scene
	{
	public:
		Scene();

		UID createEntity();
		void destroyEntity(UID entity_id);

		template<typename TComp, enable_if_component<TComp> = false>
		void addComponent(UID entity_id);
		template<typename TComp, enable_if_component<TComp> = false>
		void removeComponent(UID entity_id);

		template<typename TComp, enable_if_component<TComp> = false>
		TComp getComponent(UID entity_id);

		template<typename TEvent, enable_if_event<TEvent> = false>
		void subscribeSystem(Ref<System> system);
		template<typename TEvent, enable_if_event<TEvent> = false>
		void unsubscribeSystem(Ref<System> system);

		template<typename TEvent, enable_if_event<TEvent> = false>
		void dispatchEvent(Ref<TEvent> s_event);

	protected:

		// variable used to generate UID's for new entities
		UID m_entity_count = INVALID_ENTITY_UID + 1;
		// list containing recyclable uids
		std::queue<UID> m_avaliable_entities;

		std::vector<ComponentBuffer> m_components;

		// stores which systems are subscribed to which events
		std::map<std::type_index, Ref<System>> m_event_subscribtions;

	};
}
