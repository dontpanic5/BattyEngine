#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "UIElement.h"

class Button : UIElement
{
public:
	// TODO a callback?
	Button(const char* text, FontSize fontSize);
	Button(const char* text, FontSize fontSize, float relX, float relY);

	void SetHighlighted(bool highlighted);
	bool GetHighlighted();

	void Draw() override;
	void Draw(int posX, int posY) override;
	void Draw(int posX, int posY, bool selected);

	void ProcessInput(Input input) override;

private:
	constexpr static int S_TEXT_LENGTH = 128;
	char m_text[S_TEXT_LENGTH];
	FontSize m_fontSize;
	bool m_highlighted = false;

	bool m_externallyPositioned;
};

#endif // !BUTTON_H_INCLUDED
