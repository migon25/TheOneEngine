#ifndef __INPUT_H__
#define __INPUT_H__
#pragma once

#include "Module.h"

#define MAX_MOUSE_BUTTONS 5

enum KEY_STATE
{
    KEY_IDLE = 0,
    KEY_DOWN,
    KEY_REPEAT,
    KEY_UP
};

class Input : public Module
{
public:
    Input(App* app);

    virtual ~Input();

    bool Awake();

    bool PreUpdate();
    bool Update(double dt);

    bool processSDLEvents();

	KEY_STATE GetKey(int id) const
	{
		return keyboard[id];
	}

	KEY_STATE GetMouseButton(int id) const
	{
		return mouse_buttons[id];
	}

	int GetMouseX() const
	{
		return mouse_x;
	}

	int GetMouseY() const
	{
		return mouse_y;
	}

	int GetMouseZ() const
	{
		return mouse_z;
	}

	int GetMouseXMotion() const
	{
		return mouse_x_motion;
	}

	int GetMouseYMotion() const
	{
		return mouse_y_motion;
	}

	int GetStoredGizmoType() const
	{
		return storeGizmoType;
	}

private:
	KEY_STATE* keyboard;
	KEY_STATE mouse_buttons[MAX_MOUSE_BUTTONS];
	int mouse_x;
	int mouse_y;
	int mouse_z;
	int mouse_x_motion;
	int mouse_y_motion;
	float dx;
	float dy;

	int storeGizmoType = -1;//ImguizmoMod
};

#endif // !__INPUT_H__