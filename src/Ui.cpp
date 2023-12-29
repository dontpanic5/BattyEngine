#include "Ui.h"
#include <string.h>
#include "raylib.h"
#include "screens.h"

constexpr float SPACING = 30.0f;

void DrawUiText(Font& font, const char* text, float relX, float relY, FontSize fontSize, bool bg)
{
	int actualFontSize;

	switch (fontSize)
	{
	case FontSize::s:
		actualFontSize = GetRenderHeight() / 25;
		break;
	case FontSize::m:
		actualFontSize = GetRenderHeight() / 18;
		break;
	case FontSize::l:
	default:
		actualFontSize = GetRenderHeight() / 5;
		break;

	}

	int posX = GetRenderWidth() * relX;
	int posY = GetRenderHeight() * relY;

	Vector2 sz = MeasureTextEx(font, text, actualFontSize, actualFontSize / SPACING);

	if (bg)
	{
		DrawRectangle(posX - 20, posY - 20, sz.x + 40, sz.y + 40, {0, 0, 0, 224});
	}

	DrawTextEx(font, text, { (float)posX, (float)posY }, actualFontSize, actualFontSize / SPACING, RED);
}