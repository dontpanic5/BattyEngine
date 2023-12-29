#ifndef GROUND_H_INCLUDED
#define GROUND_H_INCLUDED

#include "EnvironmentalObject.h"

struct Texture;

class Ground : public EnvironmentalObject
{
public:
	Ground(float sz);
	~Ground();

	RayCollision GetRayCollision(Ray ray) const override;

protected:
	BoundingBox GetBoundingBox() const;

	float m_sz;

	static Texture s_tex;
	static bool s_texLoaded;
};

#endif // !GROUND_H_INCLUDED
