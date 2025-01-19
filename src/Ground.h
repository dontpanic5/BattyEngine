#ifndef GROUND_H_INCLUDED
#define GROUND_H_INCLUDED

#include "EnvironmentalObject.h"

struct Texture;

class Ground : public EnvironmentalObject
{
public:
	Ground(float sz, const char* texPath = nullptr, Color color = RAYWHITE);
	~Ground();

	RayCollision GetRayCollision(Ray ray, bool) const override;
	float getOverlapDistance(BoundingBox bb, Vector3 direction) const override;
	float getOverlapDistance(Vector3 center, float radius) const override;

	void Draw() override;

protected:
	float m_sz;

	Texture m_tex;
};

#endif // !GROUND_H_INCLUDED
