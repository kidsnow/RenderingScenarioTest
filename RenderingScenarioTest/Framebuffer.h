#pragma once

#include "GL/glew.h"

class Texture;


class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

public:
	void SetRenderTarget(Texture* texture);
	void Bind();
	void Unbind();
	void DumpFBO2PPM(const char* fileName);

private:
	void FBO_2_PPM_file(const char* filename, int width, int height);

private:
	GLuint _id;
	int _width, _height;
};