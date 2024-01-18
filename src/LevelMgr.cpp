#include "LevelMgr.h"

#include <cstdlib>
#include "BattyEngine.h"
#include "EnvironmentalObject.h"

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
	envObj->SetMaterialShader(0, g_lighting);
	m_envObjs.push_back(envObj);
}

void LevelMgr::Unload()
{
	for (EnvironmentalObject* obj : m_envObjs)
	{
		delete obj;
	}
}