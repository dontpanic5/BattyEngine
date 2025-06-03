#include <cstring>
#include "BattyEngine.h"
#include "Menu.h"
#include "Button.h"

Menu::Menu(float relX, float relY, FontSize fontSize)
	: m_fontSize(fontSize), UIElement(relX, relY)
{
	memset(m_buttons, 0, sizeof(Button*) * MAX_BUTTONS);
}

void Menu::AddButton(const char* text)
{
	m_buttons[m_numButtons++] = new Button(text, m_fontSize);
}

int Menu::GetSelection()
{
	return m_curSelection;
}

void Menu::Draw()
{
	int posX, posY;
	ConvertRelToPos(m_relX, m_relY, posX, posY);
	float actualFontSize = GetActualFontSize(m_fontSize);
	for (int i = 0; i < m_numButtons; i++)
	{
		m_buttons[i]->Draw(posX, posY + (actualFontSize + BORDER_BUFFER + 40) * i, i == m_curSelection);
	}
}

void Menu::Draw(int posX, int posY)
{
	// TODO
}

void Menu::ProcessInput(Input input)
{
	switch (input)
	{
	case Input::NONE:
		break;
	case Input::UP:
		if (m_curSelection - 1 > -1)
			m_curSelection--;
		else
			m_curSelection = m_numButtons - 1;
		break;
	case Input::DOWN:
		if (m_curSelection + 1 < m_numButtons)
			m_curSelection++;
		else
			m_curSelection = 0;
		break;
	case Input::SELECT:
		// TODO
		break;
	default:
		// never should have come here!
		_STATIC_ASSERT(true);
		break;
	}
}
