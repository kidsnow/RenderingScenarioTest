#pragma once

#include "SDLWindow.h"
#include "Shader.h"


class Application
{
protected:
	enum KEY_LIST
	{
		KEY_ENTER,
		KEY_A,
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_E,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_I,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_M,
		KEY_N,
		KEY_O,
		KEY_P,
		KEY_Q,
		KEY_R,
		KEY_S,
		KEY_T,
		KEY_U,
		KEY_V,
		KEY_W,
		KEY_X,
		KEY_Y,
		KEY_Z,
		KEY_0,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,
		KEY_LIST_SIZE
	};

public:
	Application();
	virtual ~Application();

public:
	void Run();

protected:
	virtual void Update() = 0;
	bool IsPressed(KEY_LIST code)
	{
		return _keys[(int)code];
	}

private:
	void resetKeys();

private:
	bool m_windowShouldClose;

protected:
	SDLWindow* _window;
	Shader* _shader;

private:
	bool* _keys;
};