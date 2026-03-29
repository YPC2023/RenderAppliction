#pragma once

#include "InitializeInterface.h"

class IWindowInterface : public InitializeInterface
{
public:
	typedef enum _E_MOUSE_ACTION_TYPE
	{
		E_MOUSE_ACTION_UNKOWN,
		E_MOUSE_ACTION_PRESS,
		E_MOUSE_ACTION_RELEASE,
		E_MOUSE_ACTION_MOVING,
		E_MOUSE_ACTION_WHEEL
	}E_MOUSE_ACTION_TYPE;
	typedef enum _E_MOUSE_BUTTON_TYPE
	{
		E_MOUSE_BUTTON_UNKOWN,
		E_MOUSE_BUTTON_LEFT,
		E_MOUSE_BUTTON_RIGHT,
		E_MOUSE_BUTTON_MIDDLE
	}E_MOUSE_BUTTON_TYPE;
public:
	virtual void OnRender() = 0;
	virtual void OnSize(int x, int y) = 0;
	virtual void OnMouseAction(E_MOUSE_BUTTON_TYPE key, E_MOUSE_ACTION_TYPE action, int x, int y) = 0;
	virtual void OnKeyboardAction(unsigned int key, const char* name, bool down) = 0;
};

