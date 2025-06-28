#include "Ui.h"
#include <string.h>
#include "raylib.h"
#include <cmath>
#include "BattyEngine.h"

float SPACING = 30.0f;
int BORDER_BUFFER = 40;

void ConvertRelToPos(float relX, float relY, int& posX, int& posY)
{
    posX = (int) (GetRenderWidth() * relX);
    posY = (int) (GetRenderHeight() * relY);
}

void ConvertPosToRel(float& relX, float& relY, int posX, int posY)
{
    relX = (float) posX / GetRenderWidth();
    relY = (float) posY / GetRenderHeight();
}

float GetActualFontSize(FontSize fontSize)
{
    switch (fontSize)
    {
    case FontSize::s:
        return GetRenderHeight() / 25.f;
        break;
    case FontSize::m:
        return GetRenderHeight() / 18.f;
        break;
    case FontSize::l:
    default:
        return GetRenderHeight() / 5.f;
        break;
    }
}

void GetTextBorderSz(const char* text, FontSize fontSize, int& szX, int& szY)
{
    float actualFontSize = GetActualFontSize(fontSize);

    Vector2 sz = MeasureTextEx(defaultFont, text, actualFontSize, actualFontSize / SPACING);
}

void DrawTextBorder(const char* text, int posX, int posY, FontSize fontSize, Color color)
{
    float actualFontSize = GetActualFontSize(fontSize);

    Vector2 sz = MeasureTextEx(defaultFont, text, actualFontSize, actualFontSize / SPACING);

    DrawRectangle(posX - 20, posY - 20, ((int)(sz.x)) + BORDER_BUFFER, ((int)sz.y) + BORDER_BUFFER, color);
}

void DrawUiText(const char* text, float relX, float relY, FontSize fontSize,
    bool bg, Color color, Font* font)
{
    if (font == nullptr)
        font = &defaultFont;

	float actualFontSize = GetActualFontSize(fontSize);

	int posX = (int) (GetRenderWidth() * relX);
	int posY = (int) (GetRenderHeight() * relY);

	Vector2 sz = MeasureTextEx(*font, text, actualFontSize, actualFontSize / SPACING);

	if (bg)
	{
		DrawRectangle(posX - 20, posY - 20, ((int) (sz.x)) + 40, ((int) sz.y) + 40, {0, 0, 0, 224});
	}

	DrawTextEx(*font, text, { (float)posX, (float)posY }, (float) actualFontSize, actualFontSize / SPACING, color);
}

void DrawOrb(int centerX, int centerY, float amt, Color color, float line)
{
    float radius = (float) (GetRenderWidth() / 20);
    color.a = 200;

    int leftEdge = centerX - (int) radius;

    BeginScissorMode(
        leftEdge,
        (int) (centerY - radius + ((1 - amt) * (radius * 2))),
        (int) (radius * 2),
        (int) (radius * 2)
    );
    {
        if (amt < 0.1f || amt < line + .1f && amt > line)
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

    if (line >= 0.f)
    {
        float y = centerY - radius + ((1 - line) * (radius * 2));
        DrawLine(leftEdge - (int) (radius * .1f), y, centerX + (int) (radius * 1.1f), y, RAYWHITE);
    }
}

void DrawFps()
{
}
