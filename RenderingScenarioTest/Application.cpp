#include "application.h"

#include "Camera.h"
#include <iostream>


Application::Application() :
	m_windowShouldClose(false),
	_window(nullptr),
	_camera(nullptr),
	_frustumSize(960, 540),
	_near(100.0),
	_far(1000.0),
	_mouseCurX(0),
	_mouseCurY(0),
	_mouseDeltaX(0),
	_mouseDeltaY(0),
	_wheelDeltaX(0),
	_wheelDeltaY(0)
{
	_window = new SDLWindow("Main Window", 100, 100, 960, 540);

	_keys = new bool[(int)KEY_LIST::KEY_LIST_SIZE];
	_mouseButtons = new bool[(int)MOUSE_BUTTON::MOUSE_BUTTON_SIZE];

	_camera = new Camera();
	_camera->SetProjectionMode(CPM::Orthogonal);
	_camera->SetPosition(glm::vec4(0.0, 0.0, 500.0, 1.0));
	_camera->SetFrustum(glm::vec2(960, 540), _near, _far);

	for (int i = 0; i < (int)KEY_LIST::KEY_LIST_SIZE; i++)
	{
		_keys[i] = false;
	}
	
	for (int i = 0; i < (int)MOUSE_BUTTON::MOUSE_BUTTON_SIZE; i++)
	{
		_mouseButtons[i] = false;
	}
}

Application::~Application()
{
	delete _keys;
	delete _window;
}

void Application::Run()
{
	SDL_Event event;

	while (!m_windowShouldClose)
	{
		resetKeys();
		resetMouse();
		while (SDL_PollEvent(&event))
		{
			SDL_Keycode code = event.key.keysym.sym;
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
				if (code == SDLK_ESCAPE)
				{
					m_windowShouldClose = true;
					processKeyInput();
					break;
				}
				if (code >= 'a' && code <= 'z')
				{
					int sdlkeyToKey = 'a' - KEY_A;
					_keys[code - sdlkeyToKey] = true;
					processKeyInput();
					break;
				}
				if (code >= '0' && code <= '9')
				{
					int sdlkeyToKey = '0' - KEY_0;
					_keys[code - sdlkeyToKey] = true;
					processKeyInput();
					break;
				}
				break;
			case SDL_MOUSEWHEEL:
				_wheelDeltaX = event.wheel.x;
				_wheelDeltaY = event.wheel.y;
				processMouseInput();
				break;
			case SDL_MOUSEMOTION:
				_mouseCurX = event.motion.x;
				_mouseCurY = event.motion.y;
				_mouseDeltaX = event.motion.xrel;
				_mouseDeltaY = event.motion.yrel;
				processMouseInput();
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case 1:
					_mouseButtons[MOUSE_LEFT] = true;
					break;
				case 2:
					_mouseButtons[MOUSE_WHEEL] = true;
					break;
				case 3:
					_mouseButtons[MOUSE_RIGHT] = true;
					break;
				}
				processMouseInput();
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case 1:
					_mouseButtons[MOUSE_LEFT] = false;
					break;
				case 2:
					_mouseButtons[MOUSE_WHEEL] = false;
					break;
				case 3:
					_mouseButtons[MOUSE_RIGHT] = false;
					break;
				}
				processMouseInput();
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

void Application::resetMouse()
{
	_mouseCurX   = 0;
	_mouseCurY   = 0;
	_mouseDeltaX = 0;
	_mouseDeltaY = 0;
	_wheelDeltaX = 0;
	_wheelDeltaY = 0;
}

void Application::processKeyInput()
{
	if (IsPressed(KEY_W))
	{
		_camera->Move(glm::vec3(0.0, 10.0, 0.0));
	}
	if (IsPressed(KEY_A))
	{
		_camera->Move(glm::vec3(-10.0, 0.0, 0.0));
	}
	if (IsPressed(KEY_S))
	{
		_camera->Move(glm::vec3(0.0, -10.0, 0.0));
	}
	if (IsPressed(KEY_D))
	{
		_camera->Move(glm::vec3(10.0, 0.0, 0.0));
	}
	if (IsPressed(KEY_Q))
	{
		_camera->Move(glm::vec3(0.0, 0.0, -10.0));
	}
	if (IsPressed(KEY_E))
	{
		_camera->Move(glm::vec3(0.0, 0.0, 10.0));
	}
}

void Application::processMouseInput()
{
	if (IsPressed(MOUSE_LEFT))
	{
		_camera->Move(glm::vec3(-GetDeltaOfMouseX(), GetDeltaOfMouseY(), 0.0));
	}
	float mag = GetDeltaOfWheelY() * 0.1;
	_frustumSize *= mag + 1.0;
	_camera->SetFrustum(_frustumSize, _near, _far);
}

glm::mat4 Application::GetViewProjectionMatrix()
{
	glm::mat4 view = _camera->GetInvTransform();
	glm::mat4 proj = _camera->GetProjectionTransform();

	return proj*view;
}