#pragma once

#include "SDLWindow.h"
#include "Shader.h"

class Camera;

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
	enum MOUSE_BUTTON
	{
		MOUSE_LEFT,
		MOUSE_WHEEL,
		MOUSE_RIGHT,
		MOUSE_BUTTON_SIZE
	};

public:
	Application();
	virtual ~Application();

public:
	void Run();

protected:
	virtual void Update() = 0;
	virtual void processKeyInput();
	virtual void processMouseInput();
	bool IsPressed(KEY_LIST code)
	{
		return _keys[(int)code];
	}
	bool IsPressed(MOUSE_BUTTON code)
	{
		return _mouseButtons[(int)code];
	}
	int GetDeltaOfMouseX() { return _mouseDeltaX; }
	int GetDeltaOfMouseY() { return _mouseDeltaY; }
	int GetDeltaOfWheelX() { return _wheelDeltaX; }
	int GetDeltaOfWheelY() { return _wheelDeltaY; }
	glm::mat4 GetViewProjectionMatrix();

private:
	void resetKeys();
	void resetMouse();

private:
	bool m_windowShouldClose;

protected:
	SDLWindow* _window;
	Camera* _camera;
	glm::vec2 _frustumSize;
	float _near;
	float _far;

private:
	bool* _keys;
	bool* _mouseButtons;
	int _mouseCurX;
	int _mouseCurY;
	int _mouseDeltaX;
	int _mouseDeltaY;
	int _wheelDeltaX;
	int _wheelDeltaY;
};