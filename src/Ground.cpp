#include "Ground.h"
#include "BattyUtils.h"
#include "raylib.h"
#include "raymath.h"
#ifdef PLATFORM_DESKTOP
#include <crtdbg.h>
#endif

Ground::Ground(float sz, const char* texPath, Color color)
	: m_sz(sz)
{
	Mesh mesh = GenMeshPlane(m_sz, m_sz, 2, 2);
	Init(mesh, false);
	m_bb = GetMeshBoundingBox(mesh);
	

	if (texPath)
		m_tex = LoadTexture(texPath);
	else
	{
		Image toMakeTexture = GenImageColor(1024, 1024, color);
		m_tex = LoadTextureFromImage(toMakeTexture);
	}
	SetMaterialTexture(&m_model.materials[0], MATERIAL_MAP_DIFFUSE, m_tex);
}

Ground::~Ground()
{
	UnloadTexture(m_tex);
}

RayCollision Ground::GetRayCollision(Ray ray, bool) const
{
	return GetRayCollisionQuad(ray, { m_sz, 0, m_sz }, { m_sz, 0, -m_sz }, { -m_sz, 0, -m_sz }, { -m_sz, 0, m_sz });
}

float Ground::getOverlapDistance(BoundingBox bb, Vector3 direction) const
{
	float toRet = GetOverlapDistanceBoxBox(GetBoundingBox(), bb, direction);
#ifdef PLATFORM_DESKTOP
	_ASSERT(!isnan(toRet));
#endif
	return toRet;
}

float Ground::getOverlapDistance(Vector3 center, float radius) const
{
	return GetOverlapDistanceBoxSphere(GetBoundingBox(), center, radius);
}

void Ground::Draw()
{
	EnvironmentalObject::Draw();

#ifdef DEBUG
	constexpr float SPACING = 40.0f;
	DrawGrid(m_sz / SPACING, SPACING);
#endif // DEBUG
}