#include "Texture.h"

#include <iostream>
#include "stb/stb_image.h"


Texture::Texture(std::string filePath) :
	_id(0)
{
	int width, height;

	unsigned char *data = loadImage(filePath.c_str(), width, height);

	if (data)
	{
		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFinish();

		freeImage(data);
	}
	else
	{
		std::cerr << "Failed to load texture." << std::endl;
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &_id);
}

void Texture::Render()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _id);
}

unsigned char* Texture::loadImage(const char* fileName, int& width, int& height)
{
	int temp;
	return stbi_load(fileName, &width, &height, &temp, 0);
}

void Texture::freeImage(unsigned char* data)
{
	stbi_image_free(data);
}