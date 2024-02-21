#include "EnvironmentalObject.h"
#include "raymath.h"
#include "raylib.h"

EnvironmentalObject::EnvironmentalObject()
	: m_pos(Vector3Zero())
{
}

EnvironmentalObject::EnvironmentalObject(Vector3 pos)
	: m_pos(pos)
{
}

void EnvironmentalObject::drawObj()
{
	DrawModel(m_model, m_pos, 1.0f, { 127, 127, 127, 255 });
}

void EnvironmentalObject::SetTransparent(bool transparent)
{
	m_transparent = transparent;
}

void EnvironmentalObject::SetMaterialShader(int mat, Shader shader)
{
	if (m_model.materialCount != 0)
		m_model.materials[0].shader = shader;
}

EnvironmentalObject::~EnvironmentalObject()
{
	if (m_model.materialCount != 0)
		UnloadModel(m_model);
}

bool EnvironmentalObject::IsCollidable() const
{
	return true;
}
