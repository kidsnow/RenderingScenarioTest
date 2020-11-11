#pragma once

#include "GL/glew.h"
#include <string>


class Texture
{
public:
	Texture(int width, int height);
	Texture(std::string filePath);
	~Texture();

public:
	void Render();
	int GetWidth() { return _width; }
	int GetHeight() { return _height; }
	GLuint GetID() { return _id; }

private:
	unsigned char* loadImage(const char* fileName, int& width, int& height);
	void freeImage(unsigned char* data);

private:
	GLuint _id;
	int _width, _height;
};