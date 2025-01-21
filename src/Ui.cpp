#include "Ui.h"
#include <string.h>
#include "raylib.h"
#include <cmath>
#include "BattyEngine.h"

constexpr float SPACING = 30.0f;

void DrawUiText(const char* text, float relX, float relY, FontSize fontSize,
    bool bg, Font* font)
{
	int actualFontSize;

    if (font == nullptr)
        font = &defaultFont;

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

	int posX = (int) (GetRenderWidth() * relX);
	int posY = (int) (GetRenderHeight() * relY);

	Vector2 sz = MeasureTextEx(*font, text, (float) actualFontSize, actualFontSize / SPACING);

	if (bg)
	{
		DrawRectangle(posX - 20, posY - 20, ((int) (sz.x)) + 40, ((int) sz.y) + 40, {0, 0, 0, 224});
	}

	DrawTextEx(*font, text, { (float)posX, (float)posY }, (float) actualFontSize, actualFontSize / SPACING, RED);
}

void DrawOrb(int centerX, int centerY, float amt, Color color)
{
    float radius = (float) (GetRenderWidth() / 20);
    color.a = 200;

    BeginScissorMode(
        (int) (centerX - radius),
        (int) (centerY - radius + ((1 - amt) * (radius * 2))),
        (int) (radius * 2),
        (int) (radius * 2)
    );
    {
        if (amt < 0.3f)
        {
            if (fmod(GetTime(), 0.25) > 0.125)
            {
                color.a = 0;
            }
            else
            {
                color.a = 255;
            }
        }
        DrawCircle(centerX, centerY, radius, color);
    }
    EndScissorMode();

    DrawCircleLines(
        centerX,
        centerY,
        radius,
        GRAY
    );
}

void DrawFps()
{
}
