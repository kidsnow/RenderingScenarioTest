#pragma once

#include "SDL/SDL.h"

#include "SDL/SDL_opengl.h"
#include "glm/glm.hpp"

class SDLWindow;
class Shader;


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
	virtual void Initialize() = 0;
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

	GLfloat* m_rectangleVertices;
	GLuint m_rectangleVAO;

protected:	// Common logic for test applications.
	bool InitResourcesForRectangle();
	void RenderRectangle(GLuint target);
};