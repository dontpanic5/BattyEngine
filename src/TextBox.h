#ifndef TEXTBOX_H_INCLUDED
#define TEXTBOX_H_INCLUDED

#include "UIElement.h"

class TextBox : public UIElement
{
	TextBox(bool draggable = false);

	void Draw(int posX, int posY) override;

private:
	constexpr static int S_TEXT_LENGTH = 128;
	char m_text[S_TEXT_LENGTH];
	FontSize m_fontSize;
};

#endif // !TEXTBOX_H_INCLUDED
