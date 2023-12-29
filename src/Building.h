#ifndef BUILDING_H_INCLUDED
#define BUILDING_H_INCLUDED

#include "EnvironmentalObject.h"

struct Texture;

class Building : public EnvironmentalObject
{
public:
	Building(Vector3 pos, float w, float h, float l);
	~Building();

	RayCollision GetRayCollision(Ray ray) const override;

protected:
	BoundingBox GetBoundingBox() const;

	float m_w;
	float m_h;
	float m_l;

	static bool s_texLoaded;
	static Texture s_tex;
};

#endif // !BUILDING_H_INCLUDED
