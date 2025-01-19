#ifndef ENV_OBJ_H_INCLUDED
#define ENV_OBJ_H_INCLUDED

#include "raylib.h"
#include "GeneralEntity.h"

class EnvironmentalObject : public GeneralEntity
{
public:
	void SetTransparent(bool transparent);

	virtual float getOverlapDistance(BoundingBox bb, Vector3 direction) const = 0;
	virtual float getOverlapDistance(Vector3 center, float radius) const = 0;

	virtual RayCollision GetRayCollision(Ray ray, bool addBuffer = false) const = 0;

	virtual bool IsCollidable() const;

protected:
	bool m_transparent = false;
};

#endif // !ENV_OBJ_H_INCLUDED
