#ifndef UI_ELEMENT_H_INCLUDED
#define UI_ELEMENT_H_INCLUDED

#include "Ui.h"

class UIElement
{
public:
	UIElement();
	UIElement(float relX, float relY);

	virtual void Draw() = 0;
	virtual void Draw(int posX, int posY) = 0;

	virtual void GetSize(int& szX, int& szY) = 0;

	virtual void ProcessInput(Input input) = 0;

protected:
	bool m_externallyPositioned;
	float m_relX = -1.f;
	float m_relY = -1.f;
};

#endif // !UI_ELEMENT_H_INCLUDED
