#include "ControllerMgr.h"

int chosenController = -1;

ButtonControl ControllerMgr::A = ButtonControl(7, "A", CONTROL_DEVICE::CONTROLLER);
ButtonControl ControllerMgr::B = ButtonControl(6, "B", CONTROL_DEVICE::CONTROLLER);
ButtonControl ControllerMgr::X = ButtonControl(8, "X", CONTROL_DEVICE::CONTROLLER);
AxisControl ControllerMgr::LEFT_THUMBSTICK_X = AxisControl(0, "LEFT THUMBSTICK HORIZONTAL", CONTROL_DEVICE::CONTROLLER, 1.0f, true);
AxisControl ControllerMgr::LEFT_THUMBSTICK_Y = AxisControl(1, "LEFT THUMBSTICK VERTICAL", CONTROL_DEVICE::CONTROLLER, 1.0f, false);
//ButtonControl ControllerMgr::START = ButtonControl()

ButtonControl ControllerMgr::BACKSPACE = ButtonControl(KEY_BACKSPACE, "BACKSPACE", CONTROL_DEVICE::KEYBOARD);
ButtonControl ControllerMgr::SPACEBAR = ButtonControl((int)KEY_SPACE, "SPACEBAR", CONTROL_DEVICE::KEYBOARD);
ButtonControl ControllerMgr::ENTER = ButtonControl((int)KEY_ENTER, "ENTER", CONTROL_DEVICE::KEYBOARD);
ButtonControl ControllerMgr::D = ButtonControl(KEY_D, "D", CONTROL_DEVICE::KEYBOARD);
ButtonControl ControllerMgr::F = ButtonControl(KEY_F, "F", CONTROL_DEVICE::KEYBOARD);
ButtonControl ControllerMgr::I = ButtonControl(KEY_I, "I", CONTROL_DEVICE::KEYBOARD);
ButtonControl ControllerMgr::M = ButtonControl(KEY_M, "M", CONTROL_DEVICE::KEYBOARD);
AxisControl ControllerMgr::UP_DOWN = AxisControl(KEY_UP, "UP/DOWN", CONTROL_DEVICE::KEYBOARD, 1.0f, false, KEY_DOWN);
ButtonControl ControllerMgr::UP = ButtonControl(KEY_UP, "UP", CONTROL_DEVICE::KEYBOARD);
ButtonControl ControllerMgr::DOWN = ButtonControl(KEY_DOWN, "DOWN", CONTROL_DEVICE::KEYBOARD);
AxisControl		ControllerMgr::LEFT_RIGHT = AxisControl(KEY_LEFT, "LEFT/RIGHT", CONTROL_DEVICE::KEYBOARD, 1.0f, true, KEY_RIGHT);
ButtonControl ControllerMgr::LEFT = ButtonControl(KEY_LEFT, "LEFT", CONTROL_DEVICE::KEYBOARD);
ButtonControl ControllerMgr::RIGHT = ButtonControl(KEY_RIGHT, "RIGHT", CONTROL_DEVICE::KEYBOARD);
ButtonControl ControllerMgr::ESCAPE = ButtonControl(KEY_ESCAPE, "ESCAPE", CONTROL_DEVICE::KEYBOARD);
ButtonControl ControllerMgr::ONE = ButtonControl(KEY_ONE, "ONE", CONTROL_DEVICE::KEYBOARD);
ButtonControl ControllerMgr::TWO = ButtonControl(KEY_TWO, "TWO", CONTROL_DEVICE::KEYBOARD);
ButtonControl ControllerMgr::THREE = ButtonControl(KEY_THREE, "THREE", CONTROL_DEVICE::KEYBOARD);
ButtonControl ControllerMgr::FOUR = ButtonControl(KEY_FOUR, "FOUR", CONTROL_DEVICE::KEYBOARD);
ButtonControl ControllerMgr::FIVE = ButtonControl(KEY_FIVE, "FIVE", CONTROL_DEVICE::KEYBOARD);
ButtonControl ControllerMgr::SIX = ButtonControl(KEY_SIX, "SIX", CONTROL_DEVICE::KEYBOARD);
ButtonControl ControllerMgr::SEVEN = ButtonControl(KEY_SEVEN, "SEVEN", CONTROL_DEVICE::KEYBOARD);
ButtonControl ControllerMgr::EIGHT = ButtonControl(KEY_EIGHT, "EIGHT", CONTROL_DEVICE::KEYBOARD);
ButtonControl ControllerMgr::NINE = ButtonControl(KEY_NINE, "NINE", CONTROL_DEVICE::KEYBOARD);

ButtonControl	ControllerMgr::LMB	= ButtonControl(MOUSE_BUTTON_LEFT,	"LMB",	CONTROL_DEVICE::MOUSE);
ButtonControl	ControllerMgr::RMB	= ButtonControl(MOUSE_BUTTON_RIGHT,	"RMB",	CONTROL_DEVICE::MOUSE);
AxisControl	ControllerMgr::MOUSE_UP_DOWN	= AxisControl(0,		"MOUSE UP/DOWN", CONTROL_DEVICE::MOUSE, 10.0f, false);
AxisControl	ControllerMgr::MOUSE_LEFT_RIGHT = AxisControl(1,		"MOUSE LEFT/RIGHT", CONTROL_DEVICE::MOUSE, 20.0f, true);


ButtonBinding PAUSE = ButtonBinding("PAUSE", /*TODO controller start button*/ &ControllerMgr::A, &ControllerMgr::ESCAPE, &ControllerMgr::ESCAPE);

void ControllerMgr::TrapCursor(bool trapCursor)
{
	m_trapCursor = trapCursor;
	MOUSE_UP_DOWN.TrapCursor(trapCursor);
	MOUSE_LEFT_RIGHT.TrapCursor(trapCursor);
}

Input ControllerMgr::GetInput()
{
	if ((LEFT_THUMBSTICK_Y.GetPressed() < -.8f || UP.GetPressed()) && GetTime() - m_lastInput > INPUT_WAIT)
	{
		m_lastInput = GetTime();
		return Input::UP;
	}
	else if ((LEFT_THUMBSTICK_Y.GetPressed() > .8f || DOWN.GetPressed()) && GetTime() - m_lastInput > INPUT_WAIT)
	{
		m_lastInput = GetTime();
		return Input::DOWN;
	}
	else if ((A.GetPressed() || ENTER.GetPressed()) && GetTime() - m_lastInput > INPUT_WAIT)
	{
		m_lastInput = GetTime();
		return Input::SELECT;
	}
	else
		return Input::NONE;
}
