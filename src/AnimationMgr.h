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
	static AnimationMgr& Instance();

	void Unload();

	std::unordered_map<const char*, AnimWrapper> m_animations;
};

#endif