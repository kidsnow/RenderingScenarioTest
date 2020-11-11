#include "SDLWindow.h"

#include <iostream>

SDLWindow::SDLWindow(std::string name, int posX, int posY, int width, int height) :
	_name(name),
	_posX(posX),
	_posY(posY),
	_width(width),
	_height(height)
{
	initSDLWindow();
	initGLContext();
}

SDLWindow::~SDLWindow()
{
	SDL_GL_DeleteContext(_context);
	SDL_DestroyWindow(_handle);
}

void SDLWindow::initSDLWindow()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("Couldn't initialize video driver: %s\n", SDL_GetError());
		exit(0);
	}

	uint32_t windowFlags;

	windowFlags = 0;
	windowFlags |= SDL_WINDOW_RESIZABLE;
	windowFlags |= SDL_WINDOW_OPENGL;

	_handle = SDL_CreateWindow(_name.c_str(), _posX, _posY, _width, _height, windowFlags);
	if (_handle == NULL)
	{
		SDL_Log("Couldn't create window: %s\n", SDL_GetError());
		SDL_Quit();
		exit(0);
	}

	int createdWindowWidth, createdWindowHeight;
	SDL_GetWindowSize(_handle, &createdWindowWidth, &createdWindowHeight);
	if (!(windowFlags & SDL_WINDOW_RESIZABLE) &&
		(createdWindowWidth != _width || createdWindowHeight != _height))
	{
		printf("Window requested size %dx%d, got %dx%d\n", _width, _height, createdWindowWidth, createdWindowHeight);
		_width = createdWindowWidth;
		_height = createdWindowHeight;
	}

	SDL_ShowWindow(_handle);
}

void SDLWindow::initGLContext()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	_context = SDL_GL_CreateContext(_handle);
	if (!_context)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_GL_CreateContext(): %s\n", SDL_GetError());
		SDL_DestroyWindow(_handle);
		SDL_Quit();
	}

	SDL_GL_MakeCurrent(_handle, _context);

	glewExperimental = true;
	GLenum err;
	if (err = glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to init GLEW..." << std::endl;
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(0);
	}

	std::cout << glGetString(GL_VERSION) << std::endl;
}

void SDLWindow::updateCurrentWindowSize()
{
	SDL_GetWindowSize(_handle, &_width, &_height);
}

void SDLWindow::FBO_2_PPM_file(const char* fileName, int width, int height)
{
	FILE *fp;

	unsigned char *pixels = (unsigned char*)malloc(width*height * 3);

	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	fp = fopen(fileName, "wt");
	fprintf(fp, "P3\n");
	fprintf(fp, "%d %d\n", width, height);
	fprintf(fp, "255\n");

	for (int i = height - 1; i >= 0; i--)
	{
		for (int j = 0; j < width; j++)
		{
			int idx = (i * width + j) * 3;
			fprintf(fp, "%u %u %u ",
				(unsigned int)pixels[idx],
				(unsigned int)pixels[idx + 1],
				(unsigned int)pixels[idx + 2]);
		}
	}
	fclose(fp);
	free(pixels);
}

void SDLWindow::MakeCurrent()
{
	SDL_GL_MakeCurrent(_handle, _context);
}

void SDLWindow::SwapWindow()
{
	SDL_GL_SwapWindow(_handle);
}

void SDLWindow::DumpFBO2PPM(const char* fileName)
{
	updateCurrentWindowSize();
	FBO_2_PPM_file(fileName, _width, _height);
}