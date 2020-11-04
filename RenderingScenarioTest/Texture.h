#pragma once

#include "GL/glew.h"
#include <string>


class Texture
{
public:
	Texture(std::string filePath);
	~Texture();

private:
	unsigned char* loadImage(const char* fileName, int& width, int& height);
	void freeImage(unsigned char* data);

private:
	GLuint _id;
	unsigned char* _data;
};