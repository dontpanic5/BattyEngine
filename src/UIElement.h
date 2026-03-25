#ifndef UI_ELEMENT_H_INCLUDED
#define UI_ELEMENT_H_INCLUDED

#include "Ui.h"

class UIElement
{
public:
	UIElement();
	UIElement(float relX, float relY);

	void Draw();
	virtual void Draw(int posX, int posY) = 0;

	virtual void GetSize(int& szX, int& szY) = 0;

	virtual void SetSelected(bool selected);

	virtual bool ProcessInput(Input input);

protected:
	bool m_selected = false;
	bool m_externallyPositioned;
	float m_relX = -1.f;
	float m_relY = -1.f;

	bool m_isDraggable = false;
	bool m_beingDragged = false;
};

#endif // !UI_ELEMENT_H_INCLUDED
