#include "AnimationMgr.h"
#include "raylib.h"

AnimWrapper::AnimWrapper(ModelAnimation* anims, int numOfAnims)
	: m_anims(anims), m_numOfAnims(numOfAnims)
{
}

AnimationMgr::AnimationMgr()
{
}

AnimationMgr& AnimationMgr::Instance()
{
	static AnimationMgr instance;
	return instance;
}

void AnimationMgr::LoadMultipleModelAnimations(const char** modelPaths, int numPaths)
{
    for (int i = 0; i < numPaths; i++)
    {
        int animCount;

        ModelAnimation* anims = LoadModelAnimations(modelPaths[i], &animCount);
        if (anims)
        {
            AnimationMgr::Instance().m_animations.insert({
                modelPaths[i],
                {
                    anims,
                    animCount
                }
                });
        }
    }
}

void AnimationMgr::Unload()
{
	for (auto& animW : m_animations)
	{
        UnloadModelAnimations(animW.second.m_anims, animW.second.m_numOfAnims);

#ifndef PLATFORM_WEB
#pragma warning(push)
#pragma warning(disable: 6001)
#endif // !PLATFORM_WEB
		RL_FREE(animW.second.m_anims);
#ifndef PLATFORM_WEB
#pragma warning(pop)
#endif // !PLATFORM_WEB
	}
}

int AnimationMgr::Count(const char* modelPath)
{
    return m_animations.count(modelPath);
}

AnimWrapper* AnimationMgr::GetAnim(const char* modelPath)
{
    return &m_animations.at(modelPath);
}
