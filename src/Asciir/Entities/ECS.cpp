#include "arpch.h"
#include "ECS.h"

namespace Asciir
{
    // ============ Scene definitions ============

    Entity Scene::createEntity()
    {
        return Entity(m_registry.create(), this);
    }
    
    void Scene::destroyEntity(Entity entity)
    {
        m_registry.destroy(entity.getID());
    }
}
