#include "TextBox.h"

TextBox::TextBox(bool draggable)
{
	m_isDraggable = draggable;
}

void TextBox::Draw(int posX, int posY)
{
	float relX;
	float relY;
	ConvertPosToRel(relX, relY, posX, posY);
	DrawTextBorder(m_text, posX, posY, m_fontSize, BLACK);
	DrawUiText(m_text, relX, relY, m_fontSize, false, WHITE);
}
