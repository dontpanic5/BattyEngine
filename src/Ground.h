#ifndef GROUND_H_INCLUDED
#define GROUND_H_INCLUDED

#include "EnvironmentalObject.h"

struct Texture;

class Ground : public EnvironmentalObject
{
public:
	Ground(float sz, const char* texPath);
	~Ground();

	RayCollision GetRayCollision(Ray ray) const override;

protected:
	BoundingBox GetBoundingBox() const;

	float m_sz;

	Texture m_tex;
};

#endif // !GROUND_H_INCLUDED
