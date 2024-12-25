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

	virtual bool collisionCheck(BoundingBox bb) const = 0;

	BoundingBox GetBoundingBox() const;

	virtual float getOverlapDistance(BoundingBox bb, Vector3 direction) const = 0;
	virtual float getOverlapDistance(Vector3 center, float radius) const = 0;

	virtual RayCollision GetRayCollision(Ray ray, bool addBuffer = false) const = 0;

	void SetMaterialShader(int mat, Shader shader);

	virtual ~EnvironmentalObject();

	virtual bool IsCollidable() const;

protected:
	Model m_model;

	BoundingBox m_bb;

	Vector3 m_pos;
	bool m_transparent = false;
};

#endif // !ENV_OBJ_H_INCLUDED
