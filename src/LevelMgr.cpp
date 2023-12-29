#include "LevelMgr.h"

#include <cstdlib>

#include "Ground.h"
#include "Building.h"

LevelMgr::LevelMgr()
{
}

LevelMgr& LevelMgr::Instance()
{
	static LevelMgr instance;
	return instance;
}

void LevelMgr::AddEnvObj(EnvironmentalObject* envObj)
{
	m_envObjs.push_back(envObj);
}

void LevelMgr::Unload()
{
	for (EnvironmentalObject* obj : m_envObjs)
	{
		delete obj;
	}
}