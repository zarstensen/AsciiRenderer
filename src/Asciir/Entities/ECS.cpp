#include "arpch.h"
#include "ECS.h"

namespace Asciir
{

    /*
    * ComponentBuffer definitions
    */

    uint8_t* ComponentBuffer::get(size_t indx)
    {
        AR_ASSERT_MSG(hasIndex(indx), "Invalid index was passed: ", indx, "\nSize: ", getSize(), "\nData length", dataLength());
        return unmappedGet(mapIncoming(indx));
    }

    uint8_t* ComponentBuffer::unmappedGet(size_t indx)
    {
        return &m_data[indx * (m_data_size + sizeof(size_t))];
    }

    void ComponentBuffer::set(size_t indx, uint8_t* data)
    {
        if (!hasIndex(indx))
        {
            m_data.reserve(m_data.size() + m_data_size + sizeof(size_t));

            m_data.insert(m_data.end(), data, data + m_data_size);
            m_data.insert(m_data.end(), (uint8_t*) &indx, (uint8_t*) &indx + sizeof(size_t));

            m_sparse_map[indx] = dataLength() - 1;
        }
        else
        {
            auto begin = m_data.begin() + mapIncoming(indx) * (m_data_size + sizeof(size_t));
            std::copy(data, data + m_data_size, begin);
        }
    }

    // assumes all elements outside of the new range has been removed
    // if these should automatticly be removed, call resize instead.
    void ComponentBuffer::setSize(size_t size)
    {
    #ifdef AR_DEBUG
        for (size_t i = size; i < getSize(); i++)
            AR_ASSERT_MSG(!hasIndex(i), "setSize was called with elements outside the new size range\nNew size: ", size, "\nCurrent size: ", getSize(), "Element index: ", i);
    #endif

        m_sparse_map.resize(size);
    }

    void ComponentBuffer::resize(size_t size)
    {
        for (size_t i = size; i < getSize(); i++)
            if(hasIndex(i)) remove(i);
        
        setSize(size);
    }

    bool ComponentBuffer::hasIndex(size_t indx)
    {
        for (size_t i = 0; i < dataLength(); i++)
            if (mapOutgoing(i) == indx) return true;
        
        return false;
    }

    size_t ComponentBuffer::mapOutgoing(size_t indx)
    {
        // go to the start of the target component and offset by component size to get the index relative to the sparse map
        return *(size_t*) &m_data[indx * (m_data_size + sizeof(size_t)) + m_data_size];
    }

    void ComponentBuffer::remove(size_t indx)
    {
        // offset all indexes past the element to the left in the sparse map
        for (size_t i = mapIncoming(indx) + 1; i < dataLength(); i++)
            m_sparse_map[mapOutgoing(i)]--;
         
        // remove the component from component data vector
        auto begin = m_data.begin() + mapIncoming(indx) * (m_data_size + sizeof(size_t));
        auto end = begin + (m_data_size + sizeof(size_t));
        m_data.erase(begin, end);
    }

    /*
    * System definitions
    */

    void System::run()
    {
        for (Scene* scene : m_system_scenes)
        {
            m_active_scene = scene;

            onSceneChange();

            for (ComponentView& view : scene->getView(m_system_components))
            {
                m_active_components = &view;
                process(*scene);
            }
        }
    }

    void System::addScene(Scene& scene)
    {
        // add event subscribtion information to new scene
        for (std::type_index event_id : m_subscribed_events)
        {
            scene.subscribeSystem(*this, event_id);
        }
        
        m_system_scenes.insert(&scene);
    }

    void System::removeScene(Scene& scene)
    {
        // remove all event subscribtions from removed scene
        for (std::type_index event_id : m_subscribed_events)
        {
            scene.unsubscribeSystem(*this, event_id);
        }

        m_system_scenes.erase(&scene);
    }

    /*
   * SceneView and ComponentIterator definitions
   */

    SceneView::SceneView(Scene& target_scene)
        : m_scene(target_scene) {}

    typename SceneView::ComponentIterator SceneView::begin()
    {
        return ComponentIterator(*this);
    }

    typename SceneView::ComponentIterator SceneView::end()
    {
        if (m_target_component_buffer.index() == 1)
            return ComponentIterator(*this, m_scene.componentCount(std::get<std::type_index>(m_target_component_buffer)));
        else
            return ComponentIterator(*this, 0);
    }

    void SceneView::prepare()
    {
        // find the target component buffer
        // should be the smallest component buffer of type [TComps] in the scene

        size_t min_len = SIZE_MAX;

        for (std::type_index type_index : m_required_components)
        {
            size_t component_count = m_scene.componentCount(type_index);
            if (component_count < min_len)
            {
                min_len = component_count;

                if (component_count > 0)
                    m_target_component_buffer = type_index;
                else
                {
                    // if the scene does not contain one of the required components, create an empty iterator
                    m_target_component_buffer = std::monostate();
                    break;
                }
            }
        }

    }

    SceneView::ComponentIterator::ComponentIterator(SceneView& scene_view, size_t start_index)
        : m_current_index(start_index), m_scene_view(scene_view)
    {
        findNextEntity();
    }

    typename SceneView::ComponentIterator& SceneView::ComponentIterator::operator++()
    {
        m_current_index++;
        findNextEntity();
        return *this;
    }

    typename SceneView::ComponentIterator SceneView::ComponentIterator::operator++(int)
    {
        ComponentIterator prev = *this;

        ++* this;

        return prev;
    }


    typename SceneView::ComponentIterator::reference SceneView::ComponentIterator::operator*()
    {
        return m_component_view;
    }

    bool SceneView::ComponentIterator::operator==(const ComponentIterator& other) const
    {
        return m_current_index == other.m_current_index;
    }

    bool SceneView::ComponentIterator::operator!=(const ComponentIterator& other) const
    {
        return !(*this == other);
    }

    // returns whether an entity was found in front of the current index, that contains all the required components.
    // m_current_index will automaticly be updated in the function
    bool SceneView::ComponentIterator::findNextEntity()
    {
        if (m_scene_view.m_target_component_buffer.index() == 1) // only find the next entity, if all required components exists
        {
            std::type_index component_buffer = std::get<std::type_index>(m_scene_view.m_target_component_buffer);

            Component* target_component = nullptr;
            bool found_entity = false;

            // increment index until an entity with all the passed components is found or the end of the component buffer is reached
            while (m_current_index < m_scene_view.m_scene.componentCount(component_buffer))
            {
                target_component = &m_scene_view.m_scene.getComponentIndexed(m_current_index, component_buffer);

                found_entity = true;

                for (std::type_index component : m_scene_view.m_required_components)
                {
                    if (!m_scene_view.m_scene.hasComponent(target_component->getEntUID(), component))
                    {
                        found_entity = false;
                    }
                }

                if (found_entity) break;

                m_current_index++;
            }

            // fill m_components with the current entities components only if the iterator is not an past the end iterator.
            if (found_entity)
            {
                for (std::type_index component : m_scene_view.m_required_components)
                {
                    m_component_view.set(component, m_scene_view.m_scene.getComponent(target_component->getEntUID(), component));
                }
            }

            return found_entity;
        }

        return false;
    }

    /*
    * Scene definitions
    */

    UID Scene::createEntity()
    {
        if (m_avaliable_entities.size() == 0)
        {
            m_entity_count++;

            for (auto&[_, buffer] : m_components)
                buffer.grow();

            return m_entity_count - 1;
        }
        else
        {
            UID new_uid = m_avaliable_entities.back();
            m_avaliable_entities.pop_back();

            return new_uid;
        }
    }
    
    void Scene::destroyEntity(UID entity_id)
    {
        for (auto[_,buffer] : m_components)
        {
            if (buffer.hasIndex(entity_id))
                buffer.remove(entity_id);
        }

        if (entity_id == m_entity_count - 1)
        {
            m_entity_count--;

            for (auto[_,buffer] : m_components)
                buffer.shrink();
        }
        else
            m_avaliable_entities.push_back(entity_id);
    }

    bool Scene::hasEntity(UID entity_id)
    {
        if (entity_id >= m_entity_count || entity_id == INVALID_ENTITY_UID) return false;

        for (UID invalid_id : m_avaliable_entities)
            if (invalid_id == entity_id) return false;

        return true;
    }

    Component& Scene::getComponentIndexed(size_t index, std::type_index component_type)
    {
        return *(Component*)m_components[component_type].unmappedGet(index);
    }

    Component& Scene::getComponent(UID entity_id, std::type_index component)
    {
        return *(Component*) m_components[component].get(entity_id);
    }

    SceneView Scene::getView(const std::unordered_set<std::type_index>& components)
    {
        SceneView view(*this);

        for (std::type_index id : components)
            view.addComponent(id);

        view.prepare();

        return view;
    }

    bool Scene::hasComponent(UID entity_id, std::type_index component)
    {
        // return false if component buffer does not exist
        if (componentCount(component) == 0)
            return false;

        return m_components[component].hasIndex(entity_id);
    }

    size_t Scene::componentCount(std::type_index component)
    {
        if (m_components.find(component) == m_components.end()) return 0;

        return m_components[component].dataLength();
    }

    void Scene::subscribeSystem(System& system, std::type_index event_id)
    {
        m_event_subscribtions[event_id].insert(&system);
    }

    void Scene::unsubscribeSystem(System& system, std::type_index event_id)
    {
        m_event_subscribtions[event_id].erase(&system);
    }
}
