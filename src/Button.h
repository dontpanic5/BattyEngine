#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "UIElement.h"

class Button : public UIElement
{
public:
	Button(const char* text, FontSize fontSize, UiCallback cb, float relX = -1.f, float relY = -1.f);

	void GetSize(int& szX, int& szY) override;

	void Draw() override;
	void Draw(int posX, int posY) override;
	void Draw(int posX, int posY, bool selected);

	bool ProcessInput(Input input) override;

private:
	constexpr static int S_TEXT_LENGTH = 128;
	char m_text[S_TEXT_LENGTH];
	FontSize m_fontSize;

	bool m_externallyPositioned;

	UiCallback m_cb;
};

#endif // !BUTTON_H_INCLUDED
