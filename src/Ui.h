#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

struct Font;
struct Color;

enum class FontSize { s, m, l };

void DrawUiText(const char* text, float relX, float relY, FontSize fontSize,
	bool bg = true, Font* font = nullptr);

void DrawOrb(int centerX, int centerY, float amt, Color color);

void DrawFps();

#endif // !UI_H_INCLUDED
