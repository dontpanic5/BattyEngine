#include <cstring>
#include "BattyEngine.h"
#include "Ui.h"
#include "Menu.h"
#include "Button.h"

Menu::Menu(float relX, float relY, FontSize fontSize, MenuSelectionCallback cb)
	: m_fontSize(fontSize), m_selectionCb(cb), UIElement(relX, relY)
{
	memset(m_buttons, 0, sizeof(Button*) * MAX_BUTTONS);
}

void Menu::AddButton(const char* text, UiCallback cb)
{
	m_buttons[m_numButtons++] = new Button(text, m_fontSize, cb);
}

void Menu::ClearButtons()
{
	for (int i = 0; i < m_numButtons; i++)
	{
		delete m_buttons[i];
	}

	m_numButtons = 0;

	m_curSelection = 0;
}

int Menu::GetSelection()
{
	return m_curSelection;
}

void Menu::GetSize(int& szX, int& szY)
{
	int totalX = 0, totalY = 0;
	for (int i = 0; i < m_numButtons; i++)
	{
		int buttonSzX, buttonSzY;
		m_buttons[i]->GetSize(buttonSzX, buttonSzY);
		totalY += buttonSzY;
		if (buttonSzX > totalX)
			totalX = buttonSzX;
	}
	szX = totalX;
	szY = totalY;
}

void Menu::Draw()
{
	int posX, posY;
	ConvertRelToPos(m_relX, m_relY, posX, posY);
	Draw(posX, posY);
}

void Menu::Draw(int posX, int posY)
{
	float actualFontSize = GetActualFontSize(m_fontSize);
	for (int i = 0; i < m_numButtons; i++)
	{
		m_buttons[i]->Draw(posX, posY + (actualFontSize + BORDER_BUFFER + 40) * i, m_selected ? i == m_curSelection : false);
	}
}

bool Menu::ProcessInput(Input input)
{
	switch (input)
	{
	case Input::NONE:
		return false;
		break;
	case Input::UP:
		if (m_curSelection - 1 > -1)
		{
			m_curSelection--;
			return true;
		}
		else
			return false;
		break;
	case Input::DOWN:
		if (m_curSelection + 1 < m_numButtons)
		{
			m_curSelection++;
			return true;
		}
		else
			return false;
		break;
	case Input::SELECT:
		if (m_selectionCb)
			m_selectionCb(m_curSelection);
		else
			m_buttons[m_curSelection]->ProcessInput(input);
		return true;
		break;
	default:
		// never should have come here!
		_STATIC_ASSERT(true);
		return false;
		break;
	}
}
