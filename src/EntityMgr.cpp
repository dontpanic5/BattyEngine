#include "EntityMgr.h"

EntityMgr& EntityMgr::Instance()
{
    static EntityMgr instance;
    return instance;
}

EntityMgr::EntityMgr()
{
}

void EntityMgr::ResetEntities()
{
    for (auto& entity : m_entities)
    {
        entity->ResetEntity();
    }
}

void EntityMgr::AddEntity(Entity* entity)
{
    entity->SetUid(uidIdx++);
    m_entities.push_back(entity);
}

void EntityMgr::Unload()
{
    for (Entity* entity : m_entities)
    {
        entity->UnloadEntity();
    }
}
