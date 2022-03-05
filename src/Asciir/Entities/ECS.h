#pragma once

#include "Asciir/Event/Event.h"

namespace Asciir
{
	// unique identifier datatype
	using UID = size_t;

	static constexpr UID INVALID_ENTITY_UID = std::numeric_limits<UID>::max();

	class Scene;

	// a class that can be associated with an entity and represents some data about said entity
	// should only hold data and should not be associated with any functionallity
	// acts as a base class for user and predefined components
	class Component
	{
		friend Scene;
	public:
		Component(UID uid = INVALID_ENTITY_UID)
			: m_entity_id(uid) {}

		UID getEntUID() const { return m_entity_id; }

		// cast operator to implicitly cast when adding a component to a component buffer
		operator uint8_t*() { return (uint8_t*) this; }

		bool operator==(const Component & other) { return typeid(this) == typeid(other) && getEntUID() == other.getEntUID(); }

	protected:
		UID m_entity_id;
	};

	template<typename TComp>
	using enable_if_component = enable_if_base_of<TComp, Component>;

	/*
	* class defining an entitys components and default values.
	* is passed to a Scene to quickly generate an entity with the passed components and default values.
	* 
	* all types passed must be unique and base of component
	* 
	* all arguments passed to the constructor must be of the same type and order of the types passed
	* there can be passed less arguments to the constructor than the number of types
	* 
	*/
	template<typename ... Args>
	class EntityBlueprint
	{
	public:
		// init_args must be in the same order and type as Args. Can have less InitArgs than Args, the rest of the Args will be default initialized instead.
		template<typename ... InitArgs, std::enable_if_t<sizeof...(InitArgs) <= sizeof...(Args), bool> = false>
		EntityBlueprint(InitArgs ... init_args);

		typedef std::tuple<Args...> CompList;
		const CompList& getComponentValues() const { return m_init_values; }

	protected:

		template<typename ... OArgs>
		struct are_unique;

		// check if all variadic arguments are of a unique type
		template<typename TA, typename TB, typename ... OArgs>
		struct are_unique<TA, TB, OArgs...>
		{
			//                            match TA and TB              match TA with the rest            match TB with the rest
			static constexpr bool value = are_unique<TA, TB>::value && are_unique<TA, OArgs...>::value && are_unique<TB, OArgs...>::value;
		};

		template<typename TA, typename TB>
		struct are_unique<TA, TB>
		{
			static constexpr bool value = !std::is_same_v<TA, TB>;
		};

		template<typename T>
		struct are_unique<T>
		{
			static constexpr bool value = true;
		};

		// checks for wether the class was passed the correct arguments
		static_assert((... && std::is_base_of_v<Component, Args>), "All typename arguments must be components");
		static_assert(are_unique<Args...>::value, "Cannot have more than one component on an entity");

		template<size_t index, typename InitArg, typename ... Rest,
			// check wether InitArg has the same type as the type at the current tuple index
			std::enable_if_t<std::is_same_v<InitArg, std::tuple_element_t<index, std::tuple<Args...>>>, bool> = false>
		void fillInitValues(InitArg arg, Rest ... rest);

		template<size_t index, typename InitArg,
			std::enable_if_t<std::is_same_v<InitArg, std::tuple_element_t<index, std::tuple<Args...>>>, bool> = false>
		void fillInitValues(InitArg arg);

		std::tuple<Args...> m_init_values;
	};

	// A class holding references to the components [TComps] all linked to a specific entity.
	// the individual components can be aqquired with the get function (both typename argument and std::type_index overloads exists)
	class ComponentView
	{
	public:
		ComponentView() = default;
		ComponentView(const ComponentView& other) : m_components(other.m_components) {}
		ComponentView(std::unordered_map<std::type_index, Component*> components) : m_components(components) {}

		// returns the entity id of the first component, ideally all components should have the same entity id.
		// in debug mode, it will assert if all components do not have the same id if get or getEntUID is called
		// this also means you can call the set function and still have it work, as long as all components have the same id when the view has been updated.
		UID getEntUID() {
		#ifdef AR_DEBUG
			UID last = m_components.begin()->second->getEntUID();
			for (auto [_, comp] : m_components)
				AR_ASSERT_MSG(last == comp->getEntUID(), "getEntUID was called with midex entity id's\nID1: ", last, "\nID2:", comp->getEntUID());
		#endif

			return m_components.begin()->second->getEntUID();
		}

		template<typename TComp, enable_if_component<TComp> = false>
		TComp& get() { return (TComp&) get(typeid(TComp)); }
		Component& get(std::type_index component)
		{
		#ifdef AR_DEBUG
			UID last = m_components.begin()->second->getEntUID();
			for (auto [_, comp] : m_components)
				AR_ASSERT_MSG(last == comp->getEntUID(), "get was called with midex entity id's\nID1: ", last, "\nID2:", comp->getEntUID());

			AR_ASSERT_MSG(m_components.find(component) != m_components.end(), "ComponentView does not contain a component with the type: ", component.name());
		#endif

			return *m_components[component];
		}

		template<typename TComp, enable_if_component<TComp> = false>
		void set(TComp& component) { set(typeid(TComp), component); }
		void set(std::type_index component_index, Component& component) { m_components[component_index] = &component; }

		template<typename TComp, enable_if_component<TComp> = false>
		void remove() { m_components.erase(typeid(TComp)); }
		void remove(std::type_index component_index) { m_components.erase(component_index); }

	protected:
		std::unordered_map<std::type_index, Component*> m_components;
	};

	class System;

	class SystemEvent
	{
	public:
		friend System;
		
		SystemEvent() = delete;
		SystemEvent(std::type_index event_id) : m_event_id(event_id) {}

		UID getEntUID() { return m_components->getEntUID(); }

		template<typename TComp, enable_if_component<TComp> = false>
		TComp& getComponent() { return getComponent(typeid(TComp)); }

		Component& getComponent(std::type_index component) { return m_components->get(component); }

		std::type_index getEventType() { return m_event_id; }

		Scene& getScene() { return *m_scene; }

	protected:

		ComponentView* m_components = nullptr;
		Scene* m_scene = nullptr;
		std::type_index m_event_id;
	};

	template<typename TEvent>
	using enable_if_event = enable_if_base_of<TEvent, SystemEvent>;

	/*
	a class that reads and modifies one or more components.
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
		// IMPORTANT: components returned by getComponent or a SystemEvent should not be stored as a reference, as these get overwritten when a new process call is made.
		virtual void process(Scene& scene) = 0;

		// gets called when run is called and before any process call has been made
		virtual void onStart() {}

		// gets called if run was called and there are no entities left to process
		virtual void onEnd() {}

		// gets called every time the active scene is changed.
		// a scene is only changed when every possible componentview with the required components has been processed
		virtual void onSceneChange() {}

		// gets called when an event is dispatched to the system
		virtual void onEventRecieve(SystemEvent&) {}

		// should be called by the user when the system needs to start processing entities.
		// this is not needed if the system only relies on events from other systems.
		void run();

		template<typename TComp, enable_if_component<TComp> = false>
		TComp& getComponent() { return m_active_components->get<TComp>(); }

		Scene& getScene() { return *m_active_scene; }

		UID getEntUID() { return m_active_components->getEntUID(); }

		// automaticlly creates and populates an event of specified type and dispatches it to other systems in the associated scenes.
		template<typename TEvent, enable_if_event<TEvent> = false>
		void createEvent(TEvent s_event = TEvent());


		// if multiple components are required at once, the unordered_set will be resized once instead of every time a new component is inserted
		template<typename First, typename ... Args, std::enable_if_t<(sizeof...(Args) > 0), bool> = false>
		void requireComponents();

		template<typename TComp, enable_if_component<TComp> = false>
		void requireComponents();

		template<typename First, typename ... TComps>
		void unrequireComponents();

		template<typename TComp, enable_if_component<TComp>>
		void unrequireComponents();

		// asocciates a scene with the system
		void addScene(Scene& scene);
		// unasocciates a scene with the system
		void removeScene(Scene& scene);

		// subscribe should be called whenever a scene has been added
		template<typename TEvent, enable_if_event<TEvent> = false>
		void subscribe();
		template<typename TEvent, enable_if_event<TEvent> = false>
		void unsubscribe();

	protected:

		template<typename First, typename ... Args, std::enable_if_t<(sizeof...(Args) > 0), bool> = false>
		void requireComponentsHelper();

		template<typename TComp>
		void requireComponentsHelper() { requireComponents<TComp>(); }

		// what components the system needs to function
		std::unordered_set<std::type_index> m_system_components;
		// what scenes the system is a part of
		std::unordered_set<Scene*> m_system_scenes;

		std::unordered_set<std::type_index> m_subscribed_events;

		// stores a pointer for each component currently being processed.
		ComponentView* m_active_components = nullptr;
		// UID for entity currently being processed
		UID m_active_entity;
		// stores a pointer to the current scene being processed
		Scene* m_active_scene = nullptr;
	};

	// class used by scene for storing components of same id in continous memory
	class ComponentBuffer
	{
	public:
		ComponentBuffer(size_t data_size = 0) : m_data_size(data_size) {}

		uint8_t* get(size_t indx);
		uint8_t* unmappedGet(size_t indx);
		void remove(size_t indx);
		// if a component already is stored at the current index, it is overwritten
		void set(size_t indx, uint8_t* data);

		// set the size of the sparse map without affecting the size of the data array
		void setSize(size_t size);
		// gets the size of the sparse map
		size_t getSize() { return m_sparse_map.size(); }

		// increases the size by amount
		void grow(size_t amount = 1) { setSize(getSize() + amount); }
		// decreases the size by amount
		void shrink(size_t amount = 1) { AR_ASSERT(amount <= getSize()); resize(getSize() - amount); }

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
	
	// iterable class that lets you loop over all entities in a specific scene containing specific components [TComps]
	// begin() and end() returns an ComponentIterator class
	// the iterator will not be usable after the scenes components have been modified
	class SceneView
	{
	public:

		SceneView(Scene& target_scene);

		template<typename ... TComps> // TComps can only be derived classes of Component
		SceneView(Scene& target_scene);

		template<typename TComp, enable_if_component<TComp> = false>
		void addComponent() { addComponent(typeid(TComp)); }
		void addComponent(std::type_index component_type) { m_required_components.insert(component_type); }

		template<typename TComp, enable_if_component<TComp> = false>
		void removeComponent() { removeComponent(typeid(TComp)); }
		void removeComponent(std::type_index component_type) { m_required_components.erase(component_type); }

		// should be called after components have been modified.
		// not calling the function is undefined behaviour.
		// will automaticly be called if a SceneView constructor adds components
		void prepare();

		// class for iterating over entities containing specific components
		class ComponentIterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = ComponentView;
			using difference_type = std::ptrdiff_t;
			using pointer = value_type*;
			using reference = value_type&;

			ComponentIterator(SceneView& scene_view, size_t start_index = 0);

			ComponentIterator& operator++();
			ComponentIterator operator++(int);
			bool operator==(const ComponentIterator& other) const;
			bool operator!=(const ComponentIterator& other) const;
			reference operator*();

		protected:

			bool findNextEntity();

			size_t m_current_index = 0;
			// what component buffer should be iterated over
			SceneView& m_scene_view;
			value_type m_component_view;
		};

		ComponentIterator begin();
		ComponentIterator end();

	protected:
		std::variant<std::monostate, std::type_index> m_target_component_buffer;
		std::unordered_set<std::type_index> m_required_components;
		Scene& m_scene;

	};

	// class representing a collection of entities and systems.
	// should not be inherited and should only be defined by its entities and systems attatched.
	class Scene
	{
		friend System;

	public:
		Scene() = default;

		UID createEntity();
		
		// specialization for EntityBlueprints
		// creates entity and makes sure all components are added and initialized before returning the entity uid
		template<typename TBlueprint, std::enable_if_t<enable_if_same_template<TBlueprint, EntityBlueprint>::value, bool> = false>
		UID createEntity(const TBlueprint& entity_blueprint);

		void destroyEntity(UID entity_id);

		// checks wether the passed entity_id exists in the scene
		bool hasEntity(UID entity_id);

		// get the amount of entities currently in the scene
		size_t entityCount() { return m_entity_count - m_avaliable_entities.size(); }

		template<typename TComp, enable_if_component<TComp> = false>
		void addComponents(UID entity_id, TComp component_data = TComp());

		template<typename TComp, typename ... Args, std::enable_if_t<(sizeof...(Args) > 0), bool> = false>
		void addComponents(UID entity_id);

		template<typename TComp, typename ... Args, std::enable_if_t<(sizeof...(Args) > 0), bool> = false>
		void addComponents(UID entity_id, TComp component_data, Args ... args);

		template<typename TComp, enable_if_component<TComp> = false>
		void removeComponents(UID entity_id);

		template<typename TComp, typename ... Args, std::enable_if_t<(sizeof...(Args) > 0), bool> = false>
		void removeComponents(UID entity_id);

		template<typename TComp, enable_if_component<TComp> = false>
		TComp& getComponents(UID entity_id);

		template<typename ... Args, std::enable_if_t<(sizeof...(Args) > 1), bool> = false>
		std::tuple<Args&...> getComponents(UID entity_id);

		Component& getComponent(UID entity_id, std::type_index component);

		template<typename TComp, enable_if_component<TComp> = false>
		bool hasComponent(UID entity_id) { return hasComponent(entity_id, typeid(TComp)); }

		bool hasComponent(UID entity_id, std::type_index component);

		// return the component relative to the start of the component buffer instead of mapping the entity id to an index
		template<typename TComp, enable_if_component<TComp> = false>
		TComp& getComponentIndexed(size_t index);

		Component& getComponentIndexed(size_t index, std::type_index component_type);

		template<typename ... TComps>
		SceneView getView() { return getView({ typeid(TComps)... }); };

		// return a scene view with the specified component types
		SceneView getView(const std::unordered_set<std::type_index>& components);

		// returns the amount of components of the specified type currently in the scene. If no componentbuffer exists for the passed type, a length of 0 is returned
		template<typename TComp, enable_if_component<TComp> = false>
		size_t componentCount();

		size_t componentCount(std::type_index component);

		template<typename TEvent, enable_if_event<TEvent> = false>
		void subscribeSystem(System& system) { subscribeSystem(system, typeid(TEvent)); }
		void subscribeSystem(System& system, std::type_index event_id);

		template<typename TEvent, enable_if_event<TEvent> = false>
		void unsubscribeSystem(System& system) { subscribeSystem(system, typeid(TEvent)); }
		void unsubscribeSystem(System& system, std::type_index event_id);

		template<typename TEvent, enable_if_event<TEvent> = false>
		void dispatchEvent(TEvent& s_event);

	protected:

		template<size_t index, typename TTuple>
		void readBlueprint(UID entity_id, const TTuple& blueprint_components);

		// variable used to generate UID's for new entities
		UID m_entity_count = 0;
		// list containing recyclable uids
		std::vector<UID> m_avaliable_entities;

		std::unordered_map<std::type_index, ComponentBuffer> m_components;

		// stores which systems are subscribed to which events
		std::unordered_map<std::type_index, std::unordered_set<System*>> m_event_subscribtions;

	};
}

#include "ECS.ipp"
