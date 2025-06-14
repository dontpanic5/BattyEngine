#include "Grid.h"

Grid::Grid(float relX, float relY)
	: UIElement(relX, relY)
{
}

void Grid::AddElement(UIElement* element)
{
	m_elements[m_numElements++] = element;
}

void Draw()
{

}