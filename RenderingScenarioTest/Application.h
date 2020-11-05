#pragma once

#include "SDLWindow.h"
#include "Shader.h"


class Application
{
	enum class KEY_LIST
	{
		KEY_ENTER,
		KEY_LIST_SIZE
	};

public:
	Application();
	virtual ~Application();

public:
	void Run();

protected:
	virtual void Update() = 0;

private:
	void resetKeys();

private:
	bool m_windowShouldClose;

protected:
	SDLWindow* _window;
	Shader* _shader;
	bool* _keys;
};