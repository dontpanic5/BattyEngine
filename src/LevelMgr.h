#ifndef LEVELMGR_H_INCLUDED
#define LEVELMGR_H_INCLUDED

#include <unordered_map>
#include <vector>

class EnvironmentalObject;

constexpr int MAX_REQUESTS = 16;

class LevelMgr
{
public:
	static LevelMgr& Instance();

	void AddEnvObj(EnvironmentalObject* envObj);

	void Unload();

	std::vector<EnvironmentalObject*> m_envObjs;

private:
	LevelMgr();
	LevelMgr(LevelMgr const&) = delete;
	LevelMgr& operator=(LevelMgr const&) = delete;
};

#endif