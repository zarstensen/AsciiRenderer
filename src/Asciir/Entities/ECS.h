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

		// cast operator to implicitly cast when adding a component to a component buffer
		operator uint8_t*() { return (uint8_t*) this; }

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
		Ref<TComp> getComponent() { return m_components[typeid(TComp)]; }

	protected:
		SystemEvent(std::map<std::type_index, Ref<Component>>& components, UID entity)
			: m_components(components), m_entity_id(entity) {}

		std::map<std::type_index, Ref<Component>>& m_components;
		UID m_entity_id;
	};

	template<typename TEvent>
	using enable_if_event = enable_if_base_of<TEvent, SystemEvent>;

	class Scene;

	/*
	a class that readsand modifies one or more components.
	acts as a base class for all user and library predefined systems.

	use createEvent to alert other systems subscribed to a specific SystemEvent.

	use Subscribe and Unsubscribe to define what SystemEvents will notify the system.
	
	use requireComponent and unrequireComponent to define what components the system needs to function.

	use addScene and removeScene to define what scenes the system should run on.

	*/
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
		Ref<TComp> getComponent() { return m_active_components[typeid(TComp)]; }

		// automaticlly creates and populates an event of specified type and dispatches it to other systems in the associated scenes.
		template<typename TEvent, enable_if_event<TEvent> = false>
		void createEvent();

		// if multiple components are required at once, the unordered_set will be resized once instead of every time a new component is inserted
		template<typename First, typename ... Args>
		void requireComponents();

		template<typename TComp, enable_if_component<TComp>>
		void requireComponents();

		template<typename First, typename ... TComps>
		void unrequireComponents();

		template<typename TComp, enable_if_component<TComp>>
		void unrequireComponents();

		// asocciates a scene with the system
		void addScene(Ref<Scene> scene);
		// unasocciates a scene with the system
		void removeScene(Ref<Scene> scene);

		template<typename TEvent, enable_if_event<TEvent> = false>
		void subscribe();
		template<typename TEvent, enable_if_event<TEvent> = false>
		void unsubscribe();

	protected:

		template<typename First, typename ... Args>
		void requireComponentsHelper();

		template<typename TComp>
		void requireComponentsHelper() { requireComponents<TComp>(); }

		// what components the system needs to function
		std::unordered_set<std::type_index> m_system_components;
		// what scenes the system is a part of
		std::unordered_set<Ref<Scene>> m_system_scenes;

		// stores a reference for each component currently being processed.
		std::map<std::type_index, Ref<Component>> m_active_components;
		// UID for entity currently being processed
		UID m_active_entity;
	};

	// class used by scene for storing components of same id in continous memory
	class ComponentBuffer
	{
	public:
		ComponentBuffer(size_t data_size) : m_data_size(data_size) {}

		uint8_t* get(size_t indx);
		void remove(size_t indx);
		// if a component already is stored at the current index, it is overwritten
		void set(size_t indx, uint8_t* data);

		// set the size of the sparse map without affecting the size of the data array
		void setSize(size_t size);
		// gets the size of the sparse map
		size_t getSize() { return m_sparse_map.size(); }

		// sets the size of the sparse map and removes any elements not instide the new size
		void resize(size_t size);

		// gets the number of components currently stored in the component buffer
		size_t dataLength() { return m_data.size() / (m_data_size + sizeof(size_t)); }


		// check wether the index passed has an element associated with the ComponentBuffer
		bool hasIndex(size_t indx);

		// maps an incomming index using the sparse map
		size_t mapIncoming(size_t indx) { return m_sparse_map[indx]; }
		
		// maps an index relative to m_data to the sparse_map
		size_t mapOutgoing(size_t indx);

		uint8_t* operator[](size_t indx) { return get(indx); }

	protected:

		size_t m_data_size;
		// organized as a single element being stored as [Component part (unknown bytes) + size part (4 / 8 bytes)]
		std::vector<uint8_t> m_data;
		
		std::vector<size_t> m_sparse_map;
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

#include "ECS.ipp"
