#ifndef GROUND_H_INCLUDED
#define GROUND_H_INCLUDED

#include "EnvironmentalObject.h"

struct Texture;

class Ground : public EnvironmentalObject
{
public:
	Ground(float sz, const char* texPath);
	~Ground();

	bool collisionCheck(BoundingBox bb) const override;
	RayCollision GetRayCollision(Ray ray) const override;

	void drawObj() override;

protected:
	BoundingBox GetBoundingBox() const;

	float m_sz;

	BoundingBox m_bb;

	Texture m_tex;
};

#endif // !GROUND_H_INCLUDED
