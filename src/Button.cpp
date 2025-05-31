#include <cstring>
#include <cmath>
#include "raylib.h"
#include "Ui.h"
#include "Button.h"

Button::Button(const char* text, FontSize fontSize)
	// we assume it's externally positioned because we don't get any rel values
	: m_fontSize(fontSize), m_externallyPositioned(true)
{
	strncpy(m_text, text, S_TEXT_LENGTH);
}

Button::Button(const char* text, FontSize fontSize, float relX, float relY)
	: m_fontSize(fontSize), m_externallyPositioned(false), UIElement(relX, relY)
{
	strncpy(m_text, text, S_TEXT_LENGTH);
}

void Button::SetHighlighted(bool highlighted)
{
	m_highlighted = highlighted;
}

bool Button::GetHighlighted()
{
	return m_highlighted;
}

void Button::Draw()
{
	Draw(m_relX, m_relY);
}

void Button::Draw(int posX, int posY)
{
	float relX;
	float relY;
	ConvertPosToRel(relX, relY, posX, posY);
	DrawTextBorder(m_text, posX, posY, m_fontSize);
	DrawUiText(m_text, relX, relY, m_fontSize, false, WHITE);
}

void Button::Draw(int posX, int posY, bool selected)
{
	float relX;
	float relY;
	ConvertPosToRel(relX, relY, posX, posY);
	if (selected)
	{
		double blink = std::fmod(GetTime(), 1.0);
		Color color = RED;
		// TODO use blink to change alpha of color and then set
		DrawTextBorder(m_text, posX, posY, m_fontSize);
	}
	else
		DrawTextBorder(m_text, posX, posY, m_fontSize, BLACK);
	DrawUiText(m_text, relX, relY, m_fontSize, false, WHITE);
}
