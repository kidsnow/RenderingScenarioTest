#include "application.h"

#include <iostream>


Application::Application() :
	m_windowShouldClose(false),
	_window(nullptr),
	_shader(nullptr)
{
	_window = new SDLWindow("Main Window", 100, 100, 960, 540);
	_shader = new Shader();
	_keys = new bool[(int)KEY_LIST::KEY_LIST_SIZE];
}

Application::~Application()
{
	delete _keys;
	delete _shader;
	delete _window;
}

void Application::Run()
{
	SDL_Event event;

	while (!m_windowShouldClose)
	{
		resetKeys();
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_CLOSE:
					m_windowShouldClose = true;
					break;
				}
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					m_windowShouldClose = true;
					break;
				}

				break;
			}
		}
		Update();
		_window->SwapWindow();
	}
}

void Application::resetKeys()
{
	for (int i = 0; i < (int)KEY_LIST::KEY_LIST_SIZE; i++)
	{
		_keys[i] = false;
	}
}