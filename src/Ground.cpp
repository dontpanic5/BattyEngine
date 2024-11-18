#include "Ground.h"
#include "BattyUtils.h"
#include "raylib.h"
#include "raymath.h"
#ifdef PLATFORM_DESKTOP
#include <crtdbg.h>
#endif

Ground::Ground(float sz, const char* texPath, Color color)
	: EnvironmentalObject(), m_sz(sz)
{
	Mesh mesh = GenMeshPlane(m_sz, m_sz, 2, 2);
	m_bb = GetMeshBoundingBox(mesh);
	m_model = LoadModelFromMesh(mesh);

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

bool Ground::collisionCheck(BoundingBox bb) const
{
	return CheckCollisionBoxes(bb, m_bb);
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

void Ground::drawObj()
{
	EnvironmentalObject::drawObj();

#ifdef DEBUG
	constexpr float SPACING = 40.0f;
	DrawGrid(m_sz / SPACING, SPACING);
#endif // DEBUG
}

BoundingBox Ground::GetBoundingBox() const
{
	return { {-m_sz, 0, -m_sz}, {m_sz, 0, m_sz} };
}