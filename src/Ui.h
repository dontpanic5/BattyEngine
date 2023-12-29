#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

struct Font;

enum class FontSize { s, m, l };

void DrawUiText(Font& font, const char* text, float relX, float relY, FontSize fontSize, bool bg = true);

#endif // !UI_H_INCLUDED
