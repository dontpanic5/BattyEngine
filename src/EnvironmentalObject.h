#ifndef ENV_OBJ_H_INCLUDED
#define ENV_OBJ_H_INCLUDED

#include "raylib.h"

class EnvironmentalObject
{
public:
	EnvironmentalObject();
	EnvironmentalObject(Vector3 pos);

	virtual void drawObj();

	void SetTransparent(bool transparent);

	bool collisionCheck(BoundingBox bb);

	virtual BoundingBox GetBoundingBox() const = 0;

	virtual RayCollision GetRayCollision(Ray ray) const = 0;

	virtual ~EnvironmentalObject();

protected:
	Model m_model;
	Vector3 m_pos;
	bool m_transparent = false;
};

#endif // !ENV_OBJ_H_INCLUDED
