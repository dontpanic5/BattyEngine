#include "Grid.h"

Grid::Grid(float relX, float relY)
	: UIElement(relX, relY)
{
}

void Grid::AddElement(UIElement* element)
{
	m_elements[m_numElements++] = element;

	if (m_numElements == 1)
		m_elements[0]->SetSelected(true);
}

void Grid::Clear()
{
	for (int i = 0; i < m_numElements; i++)
	{
		m_elements[i] = nullptr;
	}
	m_numElements = 0;

	m_curSelection = 0;
}

void Grid::GetSize(int& szX, int& szY)
{
	int totalX = 0, totalY = 0;
	for (int i = 0; i < m_numElements; i++)
	{
		int elementSzX, elementSzY;
		m_elements[i]->GetSize(elementSzX, elementSzY);
		totalY += elementSzY;
		if (elementSzX > totalX)
			totalX = elementSzX;
	}
	szX = totalX;
	szY = totalY;
}

void Grid::Draw()
{
	int posX, posY;
	ConvertRelToPos(m_relX, m_relY, posX, posY);
	Draw(posX, posY);
}

void Grid::Draw(int posX, int posY)
{
	int totalY = posY;
	for (int i = 0; i < m_numElements; i++)
	{
		m_elements[i]->Draw(posX, totalY + 40);
		int szX, szY;
		m_elements[i]->GetSize(szX, szY);
		totalY += szY + 40;
	}
}

bool Grid::ProcessInput(Input input)
{
	bool processed = m_elements[m_curSelection]->ProcessInput(input);
	if (processed)
		return true;

	switch (input)
	{
	case Input::NONE:
		return false;
		break;
	case Input::UP:
		if (m_curSelection - 1 > -1)
		{
			m_elements[m_curSelection]->SetSelected(false);
			m_curSelection--;
			m_elements[m_curSelection]->SetSelected(true);
			return true;
		}
		else
			return false;
		break;
	case Input::DOWN:
		if (m_curSelection + 1 < m_numElements)
		{
			m_elements[m_curSelection]->SetSelected(false);
			m_curSelection++;
			m_elements[m_curSelection]->SetSelected(true);
			return true;
		}
		else
			return false;
		break;
	default:
		break;
	}
}