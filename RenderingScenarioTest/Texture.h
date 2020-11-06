#pragma once

#include "GL/glew.h"
#include <string>


class Texture
{
public:
	Texture(std::string filePath);
	~Texture();

public:
	void Render();

private:
	unsigned char* loadImage(const char* fileName, int& width, int& height);
	void freeImage(unsigned char* data);

private:
	GLuint _id;
};