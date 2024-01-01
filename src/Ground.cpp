#include "Ground.h"
#include "raylib.h"
#include "raymath.h"

Ground::Ground(float sz, const char* texPath)
	: EnvironmentalObject(), m_sz(sz)
{
	Mesh mesh = GenMeshPlane(m_sz, m_sz, 2, 2);
	m_model = LoadModelFromMesh(mesh);

	//m_tex = LoadTexture("resources/2010-09-04_11-57-18.png");
	m_tex = LoadTexture(texPath);
	SetMaterialTexture(&m_model.materials[0], MATERIAL_MAP_DIFFUSE, m_tex);
}

Ground::~Ground()
{
	UnloadTexture(m_tex);
}

RayCollision Ground::GetRayCollision(Ray ray) const
{
	return GetRayCollisionQuad(ray, { m_sz, 0, m_sz }, { m_sz, 0, -m_sz }, { -m_sz, 0, -m_sz }, { -m_sz, 0, m_sz });
}

void Ground::drawObj()
{
	EnvironmentalObject::drawObj();

#ifdef DEBUG
	constexpr float SPACING = 20.0f;
	DrawGrid(m_sz / SPACING, SPACING);
#endif // DEBUG
}

BoundingBox Ground::GetBoundingBox() const
{
	return { {-m_sz, 0, -m_sz}, {m_sz, 0, m_sz} };
}