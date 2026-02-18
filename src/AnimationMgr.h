#ifndef ANIMMGR_H_INCLUDED
#define ANIMMGR_H_INCLUDED

#include <unordered_map>

#include "raylib.h"

struct AnimWrapper
{
	AnimWrapper(ModelAnimation* anims, int numOfAnims);

	ModelAnimation* m_anims;
	int	m_numOfAnims;
};

class AnimationMgr final
{
public:

	AnimationMgr();

	static AnimationMgr& Instance();

	void LoadMultipleModelAnimations(const char** modelPaths, int numPaths);

	void Unload();

	int Count(const char* modelPath);

	AnimWrapper* GetAnim(const char* modelPath);
private:
	AnimationMgr(AnimationMgr const&) = delete;
	AnimationMgr& operator=(AnimationMgr const&) = delete;

	std::unordered_map<const char*, AnimWrapper> m_animations;
};

#endif