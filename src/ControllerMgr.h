#ifndef CONTROLLER_MGR_H_INCLUDED
#define CONTROLLER_MGR_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include "Utils.h"
#include "RingBuffer.h"

enum class CONTROL_DEVICE { CONTROLLER, KEYBOARD, MOUSE };

class ButtonControl;
class AxisControl;
class ButtonBinding;
class AxisBinding;

class ControllerMgr
{
public:
	static ControllerMgr& Instance()
	{
		static ControllerMgr instance;
		return instance;
	}

	void SetCurControlDevice(CONTROL_DEVICE device)
	{
		m_curControlDevice = device;
	}

	CONTROL_DEVICE GetCurControlDevice() const
	{
		return m_curControlDevice;
	}

	void TrapCursor(bool trapCursor);

	static ButtonControl A;
	static ButtonControl B;
	static ButtonControl X;
	static AxisControl LEFT_THUMBSTICK_X;
	static AxisControl LEFT_THUMBSTICK_Y;

	static ButtonControl BACKSPACE;
	static ButtonControl SPACEBAR;
	static ButtonControl ENTER;
	static ButtonControl F;
	static AxisControl UP_DOWN;
	static ButtonControl UP;
	static ButtonControl DOWN;
	static AxisControl		LEFT_RIGHT;
	static ButtonControl LEFT;
	static ButtonControl RIGHT;

	static ButtonControl ESCAPE;
	static ButtonControl LMB;
	static ButtonControl RMB;
	static AxisControl	MOUSE_UP_DOWN;
	static AxisControl	MOUSE_LEFT_RIGHT;

private:
	ControllerMgr()
	{
	}
	ControllerMgr(ControllerMgr const&) = delete;
	ControllerMgr& operator=(ControllerMgr const&) = delete;

	CONTROL_DEVICE m_curControlDevice;

	bool m_trapCursor = true;
};

class Control
{
public:
	Control(int num, const char* name, CONTROL_DEVICE controlDevice)
		: m_num(num), m_controlDevice(controlDevice)
	{
		strncpy(m_name, name, _batty_countof(m_name));
		m_name[63] = (char) '\0';
	}

	const char* GetName() const
	{
		return m_name;
	}

protected:
	int m_num;
	char m_name[64];
	CONTROL_DEVICE m_controlDevice;
};

static int GetGamepad()
{
	int id = 0;
	for (int i = 0; i < 32; i++)
	{
		if (IsGamepadAvailable(i))
			id = i;
	}
	return id;
}

class ButtonControl : public Control
{
public:
	ButtonControl(int num, const char* name, CONTROL_DEVICE controlDevice)
		: Control(num, name, controlDevice)
	{
	}

	bool GetInput() const
	{
		switch (m_controlDevice)
		{
		case CONTROL_DEVICE::CONTROLLER:
			return IsGamepadButtonPressed(GetGamepad(), m_num);
		case CONTROL_DEVICE::KEYBOARD:
			return IsKeyPressed(m_num);
		case CONTROL_DEVICE::MOUSE:
			return IsMouseButtonPressed(m_num);
		}
	}
};

static void AvoidEdges()
{
	Vector2 mouse = GetMousePosition();
	constexpr float BUFFER = 0.3f;
	if (mouse.x < GetRenderWidth() * BUFFER ||
		mouse.y < GetRenderHeight() * BUFFER ||
		mouse.x > GetRenderWidth() * (1.0f - BUFFER) ||
		mouse.y > GetRenderHeight() * (1.0f - BUFFER)
	)
	{
		SetMousePosition(GetRenderWidth() / 2, GetRenderHeight() / 2);
	}
}

class AxisControl : public Control
{
public:
	AxisControl(int num, const char* name, CONTROL_DEVICE controlDevice, const float change, bool isX, int numDown = -1)
		: m_numDown(numDown), m_change(change), m_isX(isX), Control(num, name, controlDevice)
	{
	}

	float SmoothInput(float input, float weightMod)
	{
		RingBuffer<float>* pCurBuf;
		pCurBuf = m_isX ? &m_bufX : &m_bufY;

		pCurBuf->Add(input);

		float weight = 1.0f;
		float smoothInput = 0.0f;
		int i = 0;
		float totWeight = 0.0f;

		do
		{
			float stepWeight = fmax(0.0f, weight - (weightMod * i));

			smoothInput += *pCurBuf->GetAtOffsetFromHead(i++) * stepWeight;
			totWeight += stepWeight;
		} while (i < pCurBuf->GetCurSize());

		smoothInput *= 1.0f / totWeight;

		//printf("smoothInput: %f\n", smoothInput);
		return Clamp(smoothInput / m_change, -1.0f, 1.0f);
	}

	float GetInput()
	{
		float val;
		switch (m_controlDevice)
		{
		case CONTROL_DEVICE::CONTROLLER:
			return SmoothInput(GetGamepadAxisMovement(GetGamepad(), m_num), 0.8f);
		case CONTROL_DEVICE::KEYBOARD:
			if (IsKeyDown(m_num))
			{
				val = -1.0f;
			}
			else if (IsKeyDown(m_numDown))
			{
				val = 1.0f;
			}
			else
			{
				val = 0.0f;
			}

			return SmoothInput(val, 0.1f);

		case CONTROL_DEVICE::MOUSE:
			Vector2 delta = GetMouseDelta();
			//printf("delta %f %f\n", delta.x, delta.y);
			val = SmoothInput(m_isX ? delta.x : delta.y, 0.5f);

			// only check on one
			if (m_trapCursor && !m_isX)
			{
				AvoidEdges();
			}

			return val;
		}
	}

	void TrapCursor(bool trapCursor)
	{
		m_trapCursor = trapCursor;
	}

protected:
	int m_numDown;

	float m_change = 15.0f;

	bool m_isX;

	bool m_trapCursor = false;

	RingBuffer<float> m_bufX = RingBuffer<float>(10);
	RingBuffer<float> m_bufY = RingBuffer<float>(10);
};

class ButtonBinding
{
public:
	ButtonBinding(
		char* name,
		ButtonControl* controllerControl,
		ButtonControl* keyboardControl,
		ButtonControl* mouseControl
	)
		: m_controllerControl(controllerControl),
		m_keyboardControl(keyboardControl),
		m_mouseControl(mouseControl)
	{
		strncpy(m_name, name, _batty_countof(m_name) - 1);
		m_name[63] = (char) '\0';
	}

	bool GetInput() const
	{
		switch (ControllerMgr::Instance().GetCurControlDevice())
		{
		case CONTROL_DEVICE::CONTROLLER:
			return m_controllerControl->GetInput();
		case CONTROL_DEVICE::KEYBOARD:
			return m_keyboardControl->GetInput();
		case CONTROL_DEVICE::MOUSE:
			return m_mouseControl->GetInput();
		}
	}

	const char* GetName() const
	{
		return m_name;
	}

	const char* GetControlName() const
	{
		switch (ControllerMgr::Instance().GetCurControlDevice())
		{
		case CONTROL_DEVICE::CONTROLLER:
			return m_controllerControl->GetName();
		case CONTROL_DEVICE::KEYBOARD:
			return m_keyboardControl->GetName();
		case CONTROL_DEVICE::MOUSE:
			return m_mouseControl->GetName();
		}
	}

protected:
	char m_name[64];
	ButtonControl* m_controllerControl;
	ButtonControl* m_keyboardControl;
	ButtonControl* m_mouseControl;
};

class AxisBinding
{
public:
	AxisBinding(
		char* name,
		AxisControl* controllerControl, 
		AxisControl* keyboardControl,
		AxisControl* mouseControl
	)
		: m_controllerControl(controllerControl),
		m_keyboardControl(keyboardControl),
		m_mouseControl(mouseControl)
	{
		strncpy(m_name, name, _batty_countof(m_name) - 1);
		m_name[63] = (char) '\0';
	}

	float GetInput()
	{
		switch (ControllerMgr::Instance().GetCurControlDevice())
		{
		case CONTROL_DEVICE::CONTROLLER:
			return m_controllerControl->GetInput();
		case CONTROL_DEVICE::KEYBOARD:
			return m_keyboardControl->GetInput();
		case CONTROL_DEVICE::MOUSE:
			return m_mouseControl->GetInput();
		}
	}

	const char* GetName() const
	{
		return m_name;
	}

	const char* GetControlName() const
	{
		switch (ControllerMgr::Instance().GetCurControlDevice())
		{
		case CONTROL_DEVICE::CONTROLLER:
			return m_controllerControl->GetName();
		case CONTROL_DEVICE::KEYBOARD:
			return m_keyboardControl->GetName();
		case CONTROL_DEVICE::MOUSE:
			return m_mouseControl->GetName();
		}
	}

protected:
	char m_name[64];
	AxisControl* m_controllerControl;
	AxisControl* m_keyboardControl;
	AxisControl* m_mouseControl;
};

#endif // !CONTROLLER_MGR_H_INCLUDED
