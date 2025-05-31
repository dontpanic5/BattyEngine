#ifndef UI_ELEMENT_H_INCLUDED
#define UI_ELEMENT_H_INCLUDED

class UIElement
{
public:
	UIElement();
	UIElement(float relX, float relY);

	virtual void Draw() = 0;
	virtual void Draw(int posX, int posY) = 0;

protected:
	bool m_externallyPositioned;
	float m_relX = -1.f;
	float m_relY = -1.f;
};

#endif // !UI_ELEMENT_H_INCLUDED
