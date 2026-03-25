#include "UIElement.h"
#include <stdio.h>
#include "raylib.h"

UIElement::UIElement()
	: m_externallyPositioned(true)
{
}

UIElement::UIElement(float relX, float relY)
	: m_externallyPositioned(false), m_relX(relX), m_relY(relY)
{
}

void UIElement::Draw()
{
	int posX, posY;
	ConvertRelToPos(m_relX, m_relY, posX, posY);
	Draw(posX, posY);
}

void UIElement::SetSelected(bool selected)
{
	m_selected = selected;
}

bool UIElement::ProcessInput(Input input)
{
	// this base level implementation doesn't use the directional input, it only
	// checks for mouse input

	bool mousedOver = false;
	int posX = -1, posY = -1;
	ConvertRelToPos(m_relX, m_relY, posX, posY);
	int szX = -1, szY = -1;
	GetSize(szX, szY);
	Vector2 mouse = GetMousePosition();
	//printf("mouse: %f %f\n", mouse.x, mouse.y);
	if (mouse.x > (float) posX &&
		mouse.x < (float) (posX + szX) &&
		mouse.y > (float) posY &&
		mouse.y < (float) (posY + szY))
	{
		mousedOver = true;
	}

	if (mousedOver && IsMouseButtonDown(0))
	{
		m_beingDragged = true;
	}

	return false;
}