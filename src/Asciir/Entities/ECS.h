#pragma once

#include "Asciir/Event/Event.h"

namespace Asciir
{
	/// @brief unique identifier datatype
	using UID = size_t;

	static constexpr UID INVALID_ENTITY_UID = std::numeric_limits<UID>::max();

	class Scene;

	// @brief a class that can be associated with an entity and represents some data about said entity
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

	
	
	 
	/// @brief class defining an entities components and default values.
	/// is passed to a Scene to quickly generate an entity with the passed components and default values.
	///
	///	all types passed must be unique and inherit the Component class.
	///
	///	all arguments passed to the constructor must be of the same type and order, of the types passed.
	///	there can be passed less arguments to the constructor than the number of types, if this happens, the default values of the missing values will be used.
	///
	template<typename ... Args>
	class EntityBlueprint
	{
	public:
		/// @brief init_args must be in the same order and type as Args. Can have less InitArgs than Args, the rest of the Args will be default initialized instead.
		template<typename ... InitArgs, std::enable_if_t<sizeof...(InitArgs) <= sizeof...(Args), bool> = false>
		EntityBlueprint(InitArgs ... init_args);

		typedef std::tuple<Args...> CompList;
		const CompList& getComponentValues() const { return m_init_values; }

	protected:

		template<typename ... OArgs>
		struct are_unique;

		// checks if all variadic arguments are of a unique type
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

	/// @brief A class holding references to the components [TComps] all linked to a specific entity.
	/// the individual components can be aqquired with the get function (both typename argument and std::type_index overloads exists)
	class ComponentView
	{
	public:
		ComponentView() = default;
		ComponentView(const ComponentView& other) : m_components(other.m_components) {}
		ComponentView(std::unordered_map<std::type_index, Component*> components) : m_components(components) {}

		/// @brief returns the entity id of the first component, ideally all components should have the same entity id.
		/// in debug mode, it will assert if all components do not have the same id if get or getEntUID is called
		/// this also means you can call the set function with a different id, and still have it work, as long as all components have the same id when the view has been updated.
		UID getEntUID() {
		#ifdef AR_DEBUG
			UID last = m_components.begin()->second->getEntUID();
			for (auto [_, comp] : m_components)
				AR_ASSERT_MSG(last == comp->getEntUID(), "getEntUID was called with midex entity id's\nID1: ", last, "\nID2:", comp->getEntUID());
		#endif

			return m_components.begin()->second->getEntUID();
		}

		/// @brief same as get(typeid(TCmp))
		template<typename TComp, enable_if_component<TComp> = false>
		TComp& get() { return (TComp&) get(typeid(TComp)); }

		/// @brief retrieves the specified component type from the entity the view holds a reference to.
		/// @param component the component type id
		/// @return a reference to the component, contained in the entity
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

		/// @brief same as set(typeid(TComp, component))
		template<typename TComp, enable_if_component<TComp> = false>
		void set(TComp& component) { set(typeid(TComp), component); }
		
		/// @brief sets / adds the passed component index to the passed component.
		void set(std::type_index component_index, Component& component) { m_components[component_index] = &component; }

		/// @brief same as remove(typeid(TComp))
		template<typename TComp, enable_if_component<TComp> = false>
		void remove() { m_components.erase(typeid(TComp)); }

		/// @brief removes the passed component type from the entity.
		void remove(std::type_index component_index) { m_components.erase(component_index); }

	protected:
		std::unordered_map<std::type_index, Component*> m_components;
	};

	class System;

	// TODO: dont do this?
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

	
	/// @brief a class that reads and modifies one or more components.
	/// acts as a base class for all user and library predefined systems.
	///
	/// use createEvent to alert other systems subscribed to a specific SystemEvent.
	///
	/// use Subscribe and Unsubscribe to define what SystemEvents will notify the system.
	///
	/// use requireComponent and unrequireComponent to define what components the system needs to function.
	///
	/// use addScene and removeScene to define what scenes the system should run on.
	class System
	{
	public:

		System() = default;
		virtual ~System() {}

		/// @brief user implemented function.
		/// whenever a system finds an entity that contains the specified components, process is called with the entity uid and components related to the entity.
		/// events should also be created inside process.
		/// @importent components returned by getComponent or a SystemEvent should not be stored as a reference, as these get overwritten when a new process call is made.
		virtual void process(Scene& scene) = 0;

		/// @brief gets called when run is called and before any process call has been made
		virtual void onStart() {}

		/// @brief gets called if run was called and there are no entities left to process
		virtual void onEnd() {}

		/// @brief gets called every time the active scene is changed.
		/// a scene is only changed when every possible componentview with the required components has been processed
		virtual void onSceneChange() {}

		// gets called when an event is dispatched to the system
		virtual void onEventRecieve(SystemEvent&) {}

		/// @brief runs the system on all the added scenes entities.
		/// should be called by the user / system when the system needs to start processing entities.
		/// this is not needed if the system only relies on events from other systems.
		void run();

		/// @brief runs the system on a specific scenes entities.
		/// @note the scene does not need to have been added using addScene(), in order for this to work. 
		/// @param scene the scene which entities should be processed
		void run(Scene* scene);

		/// @brief gets a component from the entity that is currently being processed, should only be used inside the process() function.
		template<typename TComp, enable_if_component<TComp> = false>
		TComp& getComponent() { return m_active_components->get<TComp>(); }

		/// @brief returns the scene of which the entity currently being processed is tied to, should only be used inside the process() function.
		Scene& getScene() { return *m_active_scene; }

		/// @brief gets the id of the entity currently being processed, should only be used inside the process() function
		UID getEntUID() { return m_active_components->getEntUID(); }

		// automaticlly creates and populates an event of specified type and dispatches it to other systems in the associated scenes.
		template<typename TEvent, enable_if_event<TEvent> = false>
		void createEvent(TEvent s_event = TEvent());


		/// @brief requires that an entity must contain the specified components, before it will be processed by this system.
		/// if multiple components are required at once, the unordered_set will be resized once instead of every time a new component is inserted
		template<typename First, typename ... Args, std::enable_if_t<(sizeof...(Args) > 0), bool> = false>
		void requireComponents();

		template<typename TComp, enable_if_component<TComp> = false>
		void requireComponents();

		/// @brief removes components from the requirement list.
		template<typename First, typename ... TComps>
		void unrequireComponents();

		template<typename TComp, enable_if_component<TComp>>
		void unrequireComponents();

		/// @brief asocciates a scene with the system, meaning only entities originating from this scene (or any previously added), will be processed.
		void addScene(Scene& scene);
		/// @brief unasocciates a scene with the system
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
		std::set<Scene*> m_system_scenes;

		std::unordered_set<std::type_index> m_subscribed_events;

		// stores a pointer for each component currently being processed.
		ComponentView* m_active_components = nullptr;
		// UID for entity currently being processed
		UID m_active_entity;
		// stores a pointer to the current scene being processed
		Scene* m_active_scene = nullptr;
	};

	/// @brief class used by scene for storing components of same id in continous memory
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
	
	/// @brief iterable class that lets you loop over all entities in a specific scene containing specific components [TComps]
	/// begin() and end() returns an ComponentIterator class
	/// the iterator will not be usable after the scenes components have been modified
	class SceneView
	{
	public:
		SceneView(Scene& target_scene);

		/// @tparam TComps the components required for an entity to be iterated over.
		template<typename ... TComps> // TComps can only be derived classes of Component
		SceneView(Scene& target_scene);

		/// @brief same as addComponent(typeid(TComp))
		template<typename TComp, enable_if_component<TComp> = false>
		void addComponent() { addComponent(typeid(TComp)); }
		/// @brief adds a required component, before an entity will be iterated over.
		void addComponent(std::type_index component_type) { m_required_components.insert(component_type); }

		/// @brief same as removeComponent(typeid(TComp))
		template<typename TComp, enable_if_component<TComp> = false>
		void removeComponent() { removeComponent(typeid(TComp)); }
		/// @brief removes a required component.
		void removeComponent(std::type_index component_type) { m_required_components.erase(component_type); }

		/// @brief prepares the SceneView iterator to be iterated over.
		/// should be called after component requirements have been modified, or the scene has been modified.
		/// not calling the function is undefined behaviour.
		/// will automaticly be called if a SceneView constructor adds components
		void prepare();

		/// @brief class for iterating over entities containing specific components
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
		std::set<std::type_index> m_required_components;
		Scene& m_scene;

	};

	class Entity;

	/// @brief class representing a collection of entities and systems.
	/// should not be inherited and should only be defined by its entities and systems attatched.
	class Scene
	{
		friend System;

	public:
		Scene() = default;

		/// @brief constructs and reserves an entity UID, and returns it.
		Entity createEntity();
		
		/// @brief specialization for EntityBlueprints
		/// creates entity and makes sure all components are added and initialized before returning the entity uid
		template<typename TBlueprint, std::enable_if_t<enable_if_same_template<TBlueprint, EntityBlueprint>::value, bool> = false>
		Entity createEntity(const TBlueprint& entity_blueprint);

		/// @brief removes the passed entity_id from the scene
		void destroyEntity(UID entity);

		/// @brief checks wether the passed entity_id exists in the scene
		bool hasEntity(UID entity);

		/// @brief get the amount of entities currently in the scene
		size_t entityCount() { return m_entity_count - m_avaliable_entities.size(); }

		template<typename TComp, enable_if_component<TComp> = false>
		void addComponents(UID entity_id, TComp component_data = TComp());

		/// @brief adds the passed components to the passed entity.
		template<typename TComp, typename ... Args, std::enable_if_t<(sizeof...(Args) > 0), bool> = false>
		void addComponents(UID entity_id);

		/// @brief adds the passed components to the passed entity, and initializes them to the passed values.
		template<typename TComp, typename ... Args, std::enable_if_t<(sizeof...(Args) > 0), bool> = false>
		void addComponents(UID entity_id, TComp component_data, Args ... args);

		template<typename TComp, enable_if_component<TComp> = false>
		void removeComponents(UID entity_id);

		/// @brief removes the specified components from the entity
		template<typename TComp, typename ... Args, std::enable_if_t<(sizeof...(Args) > 0), bool> = false>
		void removeComponents(UID entity_id);

		template<typename TComp, enable_if_component<TComp> = false>
		TComp& getComponents(UID entity_id);

		/// @brief retrieves a tuple of refrences to the requiresed components.
		/// The ordering in the tuple, will be the same as the ordering in the template arguments.
		template<typename ... Args, std::enable_if_t<(sizeof...(Args) > 1), bool> = false>
		std::tuple<Args&...> getComponents(UID entity_id);

		Component& getComponent(UID entity_id, std::type_index component);

		template<typename TComp, enable_if_component<TComp> = false>
		bool hasComponent(UID entity_id) { return hasComponent(entity_id, typeid(TComp)); }

		/// @brief checks if the passed entity has the passed component.
		bool hasComponent(UID entity_id, std::type_index component);

		template<typename TComp, enable_if_component<TComp> = false>
		TComp& getComponentIndexed(size_t index);

		/// @brief retrieves the n'th element from the component buffer containing comnponents of type [component_type]
		Component& getComponentIndexed(size_t index, std::type_index component_type);

		template<typename ... TComps>
		SceneView getView() { return getView({ typeid(TComps)... }); };

		/// @brief return a scene view with the specified component requirements.
		SceneView getView(const std::unordered_set<std::type_index>& components);

		template<typename TComp, enable_if_component<TComp> = false>
		size_t componentCount();

		/// @brief returns the amount of components of the specified type currently in the scene.
		/// If no componentbuffer exists for the passed type, a length of 0 is returned
		size_t componentCount(std::type_index component);

		/// @brief associates a System to the current Scene.
		/// if a system is associated witha  scene, it will be run, on the current Scene, when the runSystem() method is called.
		/// @note this does not modify the system itself in any way, meaning after this call, a call to system.run(),
		/// will not include this scene, unless it was added with system.addScene.
		void addSystem(System& system) { m_associated_systems.insert(&system); }

		/// @brief checks if the passed system is associated with the current scene.
		/// @param system 
		bool hasSystem(System& system) { return m_associated_systems.find(&system) != m_associated_systems.end(); }

		/// @brief unassociates a System witht he current scene.
		void removeSystem(System& system) { AR_ASSERT_MSG(hasSystem(system), "Scene must contain system before it can be removed!"); m_associated_systems.erase(&system); }
		
		/// @brief runs all systems associated with the current scene.
		void runSystems()
		{
			for (System* system : m_associated_systems)
				system->run(this);
		}

		// TODO: should events be a thing?
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

		std::unordered_set<System*> m_associated_systems;

		std::map<std::type_index, ComponentBuffer> m_components;

		// stores which systems are subscribed to which events
		std::map<std::type_index, std::set<System*>> m_event_subscribtions;

	};

	/// @brief the entity class responsible for managing a single entity and information handling to its scene
	/// the primary purpose of this class is to avoid constantly having to refrence the scene an entity is tied to, when adding, removing and retrieving components.
	/// 
	/// this means that functions like Scene::addComponents has an equivalent Entity::add, which does exactly the same as the former function.
	/// However, the scene does not have to be specified, as the Entity class also stores which scene the entity is tied to.
	/// 
	class Entity
	{
	public:

		Entity() = default;

		/// @brief retrieve a component from the entity.
		template<typename TComp>
		TComp& get()
		{
			return m_scene->getComponents<TComp>(getID());
		}

		/// @brief checks if the entity has the specified component(s).
		template<typename TComp, typename ... Rest>
		bool has()
		{
			if constexpr (sizeof...(Rest) == 0)
			{
				return m_scene->hasComponent<TComp>();
			}
			else
			{
				if (m_scene->hasComponent<TComp>())
					return has<Rest...>();
				else
					return false;
			}
		}

		template<typename TComp, typename ... Rest, std::enable_if_t<(sizeof...(Rest) > 0), bool> = false>
		void add()
		{
			m_scene->addComponents<TComp, Rest...>(getID());
		}

		/// @brief adds the specified component(s) to the entity. Optionally, initial values can be passed, with the same logic as Scene::addComponents.
		template<typename ... TComps>
		void add(const TComps& ... component_vals)
		{
			m_scene->addComponents<TComp, Rest...>(getID(), component_vals...);
		}

		/// @brief removes the speciffied component(s) from the entity.
		template<typename ... TComps>
		void remove()
		{
			m_scene->removeComponents<TComps...>(getID());
		}

		/// @brief removes the entity from its associated scene.
		/// this will deallocate all its components and mark its id as avaliable.
		/// As well as this, m_id will be set to INVALID_ENTITY_UID and m_scene will be set to a nullptr.
		void destroy()
		{
			m_scene->destroyEntity(getID());

			m_id = INVALID_ENTITY_UID;
			m_scene = nullptr;
		}

		/// @return returns the id of the entity,
		UID getID() { return m_id; }

		/// @return returns the scene that the entity is tied to.
		Scene* getScene() { return m_scene; }

	protected:
		Entity(UID id, Scene* scene)
			: m_id(id), m_scene(scene) {}

		UID m_id = INVALID_ENTITY_UID;
		Scene* m_scene = nullptr;
		friend Scene;
	};
}

#include "ECS.ipp"
