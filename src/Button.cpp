#include <cstring>
#include <cmath>
#include "raylib.h"
#include "BattyEngine.h"
#include "Ui.h"
#include "Button.h"

Button::Button(const char* text, FontSize fontSize, UiCallback cb, float relX, float relY)
	: m_fontSize(fontSize), m_externallyPositioned(false), m_cb(cb), UIElement(relX, relY)
{
	if (relX > 0.f && relY > 0.f)
		m_externallyPositioned = false;
	else
		m_externallyPositioned = true;
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

void Button::GetSize(int& szX, int& szY)
{
	float actualFontSize = GetActualFontSize(m_fontSize);
	Vector2 sz = MeasureTextEx(defaultFont, m_text, actualFontSize, actualFontSize / SPACING);
}

void Button::Draw()
{
	int posX, posY;
	ConvertRelToPos(m_relX, m_relY, posX, posY);
	Draw(posX, posY);
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
	Color bg;
	if (selected)
	{
		bg = RED;
	}
	else
		bg = BLACK;
	DrawTextBorder(m_text, posX, posY, m_fontSize, bg);

	Color color = WHITE;
	if (selected)
	{
		double blink = std::fmod(GetTime(), .75);
		// use blink to change alpha of color
		color.a -= blink * 100;
	}

	DrawUiText(m_text, relX, relY, m_fontSize, false, color);
}

bool Button::ProcessInput(Input input)
{
	if (input == Input::SELECT)
	{
		m_cb();
		return true;
	}
	else
		return false;
}
