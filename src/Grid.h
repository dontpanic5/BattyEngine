#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

#include "UIElement.h"

class Grid : public UIElement
{
public:
	Grid(float relX, float relY);

	void AddElement(UIElement* element);
	void Clear();

	void GetSize(int& szX, int& szY) override;

	void Draw() override;
	void Draw(int posX, int posY) override;

	bool ProcessInput(Input input) override;

private:
	int m_curSelection = 0;

	constexpr static int MAX_ELEMENTS = 16;
	UIElement* m_elements[MAX_ELEMENTS];
	int m_numElements = 0;
};

#endif // !GRID_H_INCLUDED
