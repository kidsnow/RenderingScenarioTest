#pragma once

#include <string>
#include "SDL/SDL.h"
#include "GL/glew.h"


class SDLWindow
{
public:
	SDLWindow(std::string name, int posX, int posY, int width, int height);
	~SDLWindow();

private:
	void initSDLWindow();
	void initGLContext();
	void updateCurrentWindowSize();
	void FBO_2_PPM_file(const char* fileName, int width, int height);

public:
	void MakeCurrent();
	void SwapWindow();
	void DumpFBO2PPM(const char* fileName);

private:
	SDL_Window* _handle;
	SDL_GLContext _context;
	std::string _name;
	int _posX, _posY;
	int _width, _height;
};