#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

#include "UIElement.h"

class Grid : UIElement
{
public:
	Grid(float relX, float relY);

	void AddElement(UIElement* element);

	void Draw() override;
	void Draw(int posX, int posY) override;

	void ProcessInput(Input input) override;

private:
	constexpr static int MAX_ELEMENTS = 16;
	UIElement* m_elements[MAX_ELEMENTS];
	int m_numElements = 0;
};

#endif // !GRID_H_INCLUDED
