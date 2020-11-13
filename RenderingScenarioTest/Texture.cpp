#include "Texture.h"

#include <iostream>
#include "stb/stb_image.h"


Texture::Texture(int width, int height) :
	_id(0),
	_unitIndex(0),
	_width(width),
	_height(height)
{
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFinish();

}

Texture::Texture(std::string filePath) :
	_id(0),
	_width(0),
	_height(0)
{
	unsigned char *data = loadImage(filePath.c_str(), _width, _height);
	
	if (data)
	{
		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
	glActiveTexture(GL_TEXTURE0 + _unitIndex);
	glBindTexture(GL_TEXTURE_2D, _id);
}

unsigned char* Texture::loadImage(const char* fileName, int& width, int& height)
{
	int temp;
	return stbi_load(fileName, &width, &height, &temp, STBI_rgb);
}

void Texture::freeImage(unsigned char* data)
{
	stbi_image_free(data);
}