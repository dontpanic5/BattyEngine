#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "Ui.h"
#include "UIElement.h"
class Button;

typedef void (*MenuSelectionCallback)(int);

class Menu : UIElement
{
public:
	Menu(float relX, float relY, FontSize fontSize, MenuSelectionCallback selectionCb = nullptr);

	void AddButton(const char* text, UiCallback cb = nullptr);

	void ClearButtons();

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

	MenuSelectionCallback m_selectionCb = nullptr;
};

#endif // !MENU_H_INCLUDED