#pragma once

#include "Asciir/Event/Event.h"
#include <entt/entt.hpp>

namespace Asciir
{
	class Scene;

	class Entity;
	template<typename ... Args>
	class EntityBlueprint;

	class System;

	using SystemPtr = void(*)(System*, Scene&);

	class System
	{
	public:
		System(SystemPtr main_func)
			: m_system(main_func) {}

		System() = default;

		void run(Scene& scene)
		{
			m_system(this, scene);
		}

	protected:
		SystemPtr m_system;

	};

	/// @brief class representing a collection of entities and systems.
	/// should not be inherited and should only be defined by its entities and systems attatched.
	// TODO: use entt::registry underneath
	class Scene
	{
	public:
		Scene() = default;

		/// @brief constructs and reserves an entity UID, and returns it.
		Entity createEntity();
		
		/// @brief specialization for EntityBlueprints
		/// creates entity and makes sure all components are added and initialized before returning the entity uid
		template<typename TBlueprint, std::enable_if_t<enable_if_same_template<TBlueprint, EntityBlueprint>::value, bool> = false>
		Entity createEntity(const TBlueprint& entity_blueprint);

		/// @brief removes the passed entity_id from the scene
		void destroyEntity(Entity entity);

		/// @brief associates a System to the current Scene.
		/// if a system is associated witha  scene, it will be run, on the current Scene, when the runSystem() method is called.
		/// @note this does not modify the system itself in any way, meaning after this call, a call to system.run(),
		/// will not include this scene, unless it was added with system.addScene.
		void addSystem(Ref<System> system) { m_associated_systems.push_back(system); }

		/// @brief checks if the passed system is associated with the current scene.
		bool hasSystem(Ref<System> system) { return std::find(m_associated_systems.begin(), m_associated_systems.end(), system) != m_associated_systems.end(); }

		/// @brief unassociates a System witht he current scene.
		void removeSystem(Ref<System> system) { AR_ASSERT_MSG(hasSystem(system), "Scene must contain system before it can be removed!"); m_associated_systems.erase(std::find(m_associated_systems.begin(), m_associated_systems.end(), system)); }
		
		/// @brief runs all systems associated with the current scene.
		void runSystems()
		{
			for (Ref<System> system : m_associated_systems)
				system->run(*this);
		}

		entt::registry& getReg()
		{
			return m_registry;
		}

	protected:

		template<size_t index, typename TTuple>
		void readBlueprint(Entity entity_id, const TTuple& blueprint_components);

		std::vector<Ref<System>> m_associated_systems;

		entt::registry m_registry;
	};

	/// @brief class defining an entities components and default values.
	/// is passed to a Scene to quickly generate an entity with the passed components and default values.
	///
	///	all types passed must be unique and inherit the Component class.
	///
	///	all arguments passed to the constructor must be of the same type and order, of the types passed.
	///	there can be passed less arguments to the constructor than the number of types, if this happens, the default values of the missing values will be used.
	///
	// TODO: keep this
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

	/// @brief the Entity class responsible for managing a single entity and its copmonents.
	/// the primary purpose of this class is to avoid constantly having to refrence the scene an entity is tied to, when adding, removing and retrieving components.
	/// 
	/// this means that functions like Scene::addComponents has an equivalent Entity::add, which does exactly the same as the former function.
	/// However, the scene does not have to be specified, as the Entity class also stores which Scene the entity is tied to.
	// TODO: use entt::Entity and Scene underneath
	class Entity
	{
	public:

		Entity() = default;
		// creates a reference to the passed entity
		Entity(const Entity& other)
			: m_id(other.m_id), m_scene(other.m_scene) {}

		/// @brief retrieve a component from the entity.
		template<typename TComp>
		TComp& get()
		{
			return m_scene->getReg().get<TComp>(m_id);
		}

		/// @brief checks if the entity has the specified component(s).
		template<typename ... TComps>
		bool has()
		{
			return m_scene->getReg().all_off<TComps...>(m_id);
		}

		template<typename TComp, typename ... Rest>
		void add()
		{
			m_scene->getReg().emplace<TComp>(m_id);
			
			if constexpr (sizeof...(Rest) > 0)
				add<Rest...>();
		}

		/// @brief adds the specified component(s) to the entity. Optionally, initial values can be passed, with the same logic as Scene::addComponents.
		template<typename TComp, typename ... Rest>
		void add(const TComp& val, const Rest& ... rest)
		{
			m_scene->getReg().emplace<TComp>(m_id);
			if (sizeof...(Rest) > 0)
				add<Rest...>(rest...);
		}

		/// @brief removes the speciffied component(s) from the entity.
		template<typename TComp, typename ... Rest>
		void remove()
		{
			m_scene->getReg().remove<TComp>(m_id);

			if (sizeof...(Rest) > 0)
				remove<Rest...>();
		}

		/// @brief removes the entity from its associated scene.
		/// this will deallocate all its components and mark its id as avaliable.
		/// As well as this, m_id will be set to INVALID_ENTITY_UID and m_scene will be set to a nullptr.
		void destroy()
		{
			m_scene->destroyEntity(*this);

			m_id = entt::null;
		}

		/// @return returns the id of the entity.
		entt::entity getID() const { return m_id; }

		/// @return returns the scene that the entity is tied to.
		Scene& getScene() const { return *m_scene; }

		operator bool() { return m_id != entt::null && &m_scene != nullptr; }

		Entity& operator=(const Entity& other)
		{
			m_id = other.getID();
			m_scene = &other.getScene();

			return *this;
		}

	protected:
		Entity(entt::entity id, Scene* scene)
			: m_id(id), m_scene(scene) {}

		entt::entity m_id;
		Scene* m_scene;

		friend Scene;
	};
}

#include "ECS.ipp"
