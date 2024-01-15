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
	unsigned char alpha = m_transparent ? 110 : 255;
	DrawModel(m_model, m_pos, 1.0f, { 127, 127, 127, alpha });
}

void EnvironmentalObject::SetTransparent(bool transparent)
{
	m_transparent = transparent;
}

bool EnvironmentalObject::collisionCheck(BoundingBox bb)
{
	return CheckCollisionBoxes(GetBoundingBox(), bb);
}

void EnvironmentalObject::SetMaterialShader(int mat, Shader shader)
{
	m_model.materials[0].shader = shader;
}

EnvironmentalObject::~EnvironmentalObject()
{
	UnloadModel(m_model);
}
