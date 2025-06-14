#include "UIElement.h"

UIElement::UIElement()
	: m_externallyPositioned(true)
{
}

UIElement::UIElement(float relX, float relY)
	: m_externallyPositioned(false), m_relX(relX), m_relY(relY)
{
}

void UIElement::SetSelected(bool selected)
{
	m_selected = selected;
}
