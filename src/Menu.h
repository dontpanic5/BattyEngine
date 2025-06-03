#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "Ui.h"
#include "UIElement.h"
class Button;

class Menu : UIElement
{
public:
	Menu(float relX, float relY, FontSize fontSize);

	// TODO maybe a callback for when it's selected?
	void AddButton(const char* text);

	int GetSelection();

	void Draw() override;
	void Draw(int posX, int posY) override;

	void ProcessInput(Input input) override;

private:
	int m_numButtons = 0;
	int m_curSelection = 0;

	constexpr static int MAX_BUTTONS = 16;
	Button* m_buttons[MAX_BUTTONS];

	FontSize m_fontSize;
};

#endif // !MENU_H_INCLUDED