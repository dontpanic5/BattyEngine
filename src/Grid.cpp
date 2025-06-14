#include "Grid.h"

Grid::Grid(float relX, float relY)
	: UIElement(relX, relY)
{
}

void Grid::AddElement(UIElement* element)
{
	m_elements[m_numElements++] = element;
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
	switch (input)
	{
	case Input::NONE:
		return false;
		break;
	case Input::UP:
		// TODO
		break;
	case Input::DOWN:
		break;
	case Input::SELECT:
		break;
	default:
		break;
	}
}