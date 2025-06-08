#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#include "raylib.h"

enum class FontSize { s, m, l };

typedef void (*UiCallback)();

void ConvertRelToPos(float relX, float relY, int& posX, int& posY);
void ConvertPosToRel(float& relX, float& relY, int posX, int posY);

float GetActualFontSize(FontSize fontSize);

void DrawTextBorder(const char* text, int posX, int posY, FontSize fontSize, Color color = RED);

void DrawUiText(const char* text, float relX, float relY, FontSize fontSize,
	bool bg = true, Color color = RED, Font* font = nullptr);

void DrawOrb(int centerX, int centerY, float amt, Color color);

void DrawFps();

extern int BORDER_BUFFER;

enum class Input
{
	NONE,
	UP,
	DOWN,
	SELECT,
};

#endif // !UI_H_INCLUDED
