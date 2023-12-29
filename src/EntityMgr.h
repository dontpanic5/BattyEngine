#ifndef ENTITYMGR_H_INCLUDED
#define ENTITYMGR_H_INCLUDED

#include <vector>

#include "Entity.h"

class EntityMgr
{
public:
	static EntityMgr& Instance();

	void ResetEntities();

	void AddEntity(Entity* entity);

	void Unload();

	std::vector<Entity*> m_entities;

private:
	EntityMgr();
	EntityMgr(EntityMgr const&) = delete;
	EntityMgr& operator=(EntityMgr const&) = delete;

	int uidIdx = 0;
};

#endif