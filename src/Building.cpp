#include "Building.h"
#include "raylib.h"

bool Building::s_texLoaded = false;
Texture Building::s_tex;

Building::Building(Vector3 pos, float w, float h, float l)
	: EnvironmentalObject(pos), m_w(w), m_h(h), m_l(l)
{
	// make it so we can define buildings by where their base is
	m_pos.y += h/2;

	Mesh mesh = GenMeshCube(w, h, l);
	m_model = LoadModelFromMesh(mesh);

	if (!s_texLoaded)
		s_tex = LoadTexture("resources/building_dock_apartments.png");
	SetMaterialTexture(&m_model.materials[0], MATERIAL_MAP_DIFFUSE, s_tex);
}

Building::~Building()
{
	if (s_texLoaded)
	{
		UnloadTexture(s_tex);
		s_texLoaded = false;
	}
}

RayCollision Building::GetRayCollision(Ray ray) const
{
	return GetRayCollisionBox(ray, GetBoundingBox());
}

BoundingBox Building::GetBoundingBox() const
{
	return {
			{m_pos.x - m_w / 2, m_pos.y - m_h / 2, m_pos.z - m_l / 2},
			{m_pos.x + m_w / 2, m_pos.y + m_h / 2, m_pos.z + m_l / 2}
	};
}
