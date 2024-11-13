#include "EntityMgr.h"
#include "BattyEngine.h"

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
    entity->SetMaterialShaders(g_skinning);
    m_entities.push_back(entity);
}

void EntityMgr::Unload()
{
    m_entities.clear();
}
