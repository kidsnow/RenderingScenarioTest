#include "Framebuffer.h"
#include "Texture.h"


Framebuffer::Framebuffer() :
	_id(0),
	_width(0),
	_height(0)
{
	glGenFramebuffers(1, &_id);
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &_id);
}

void Framebuffer::SetRenderTarget(Texture* texture)
{
	_width = texture->GetWidth();
	_height = texture->GetHeight();
	glBindFramebuffer(GL_FRAMEBUFFER, _id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetID(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _id);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		exit(0);
	}
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::DumpFBO2PPM(const char* fileName)
{
	FBO_2_PPM_file(fileName, _width, _height);
}

void Framebuffer::FBO_2_PPM_file(const char* fileName, int width, int height)
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