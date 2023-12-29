#include "Ground.h"
#include "raylib.h"
#include "raymath.h"

bool Ground::s_texLoaded = false;
Texture Ground::s_tex;

Ground::Ground(float sz)
	: EnvironmentalObject(), m_sz(sz)
{
	Mesh mesh = GenMeshPlane(m_sz, m_sz, 2, 2);
	m_model = LoadModelFromMesh(mesh);

	if (!s_texLoaded)
		s_tex = LoadTexture("resources/2010-09-04_11-57-18.png");
	SetMaterialTexture(&m_model.materials[0], MATERIAL_MAP_DIFFUSE, s_tex);
}

Ground::~Ground()
{
	if (s_texLoaded)
	{
		UnloadTexture(s_tex);
		s_texLoaded = false;
	}
}

RayCollision Ground::GetRayCollision(Ray ray) const
{
	return GetRayCollisionQuad(ray, { m_sz, 0, m_sz }, { m_sz, 0, -m_sz }, { -m_sz, 0, -m_sz }, { -m_sz, 0, m_sz });
}

BoundingBox Ground::GetBoundingBox() const
{
	return { {-m_sz, 0, -m_sz}, {m_sz, 0, m_sz} };
}