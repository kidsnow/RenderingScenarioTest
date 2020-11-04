#include "application.h"

#include <iostream>
#include "stb/stb_image.h"
#include "SDLWindow.h"
#include "Shader.h"

Application::Application() :
	m_windowShouldClose(false),
	_window(nullptr),
	_shader(nullptr)
{
	_window = new SDLWindow("Main Window", 100, 100, 960, 540);
	_shader = new Shader();
	_keys = new KEY_LIST[(int)KEY_LIST::KEY_LIST_SIZE];
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

bool Application::InitResourcesForRectangle()
{
	m_rectangleVertices = new GLfloat[30]
	{
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,	// lower left
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	// lower right
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,	// upper left
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,	// upper left
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	// lower right
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,	// upper right
	};
	m_shader = new Shader();
	m_shader->CompileRenderingShader("simple.vert", "simple.frag");

	// Initialize vertex array object.
	glGenVertexArrays(1, &m_rectangleVAO);
	glBindVertexArray(m_rectangleVAO);

	// Initialize vertex buffer object.
	GLuint rectangleVBO;
	glGenBuffers(1, &rectangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, rectangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 5, m_rectangleVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, rectangleVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, rectangleVBO);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

void Application::RenderRectangle(GLuint target)
{
	glBindFramebuffer(GL_FRAMEBUFFER, target);
	glClearColor(0.4f, 0.4f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(m_rectangleVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_srcTexture);

	m_shader->Use();

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glFinish();

	return;
}