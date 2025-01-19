#include "EnvironmentalObject.h"
#include "raymath.h"
#include "raylib.h"

void EnvironmentalObject::SetTransparent(bool transparent)
{
	m_transparent = transparent;
}

bool EnvironmentalObject::IsCollidable() const
{
	return true;
}
