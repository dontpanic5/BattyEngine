#ifndef GROUND_H_INCLUDED
#define GROUND_H_INCLUDED

#include "EnvironmentalObject.h"

struct Texture;

class Ground : public EnvironmentalObject
{
public:
	Ground(float sz, const char* texPath = nullptr, Color color = RAYWHITE);
	~Ground();

	bool collisionCheck(BoundingBox bb) const override;
	RayCollision GetRayCollision(Ray ray, bool) const override;
	float getOverlapDistance(BoundingBox bb, Vector3 direction) const override;

	BoundingBox GetBoundingBox() const override;

	void drawObj() override;

protected:
	float m_sz;

	BoundingBox m_bb;

	Texture m_tex;
};

#endif // !GROUND_H_INCLUDED
