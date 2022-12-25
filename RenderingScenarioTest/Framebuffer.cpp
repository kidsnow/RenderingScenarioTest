#include "Framebuffer.h"

#include "GL/glew.h"
#include <fstream>

Framebuffer::Framebuffer(int _width, int _height, int _sampleCount, BufferType _bufferType) :
	m_width(_width),
	m_height(_height),
	m_bufferType(_bufferType),
	m_sampleCount(_sampleCount),
	m_bufferID(0),
	m_bufferShared(false),
	m_framebufferID(0)
{
}

Framebuffer::~Framebuffer()
{
	if (m_bufferID != 0)
	{
		if (m_bufferType == BufferType::Renderbuffer)
		{
			glDeleteRenderbuffers(1, (GLuint*)&m_bufferID);
		}
		else if (m_bufferType == BufferType::Texture)
		{
			if (m_bufferShared == false)
			{
				glDeleteTextures(1, (GLuint*)&m_bufferID);
			}
		}
		m_bufferID = 0;
	}
	if (m_framebufferID != 0)
	{
		glDeleteFramebuffers(1, (GLuint*)&m_framebufferID);
		m_framebufferID = 0;
	}
}

bool Framebuffer::Initialize()
{
	if (m_bufferType == BufferType::Renderbuffer)
	{
		// Generate renderbuffer
		{
			glGenRenderbuffers(1, &m_bufferID);
			glBindRenderbuffer(GL_RENDERBUFFER, m_bufferID);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_sampleCount, GL_RGBA8, m_width, m_height);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}

		// Generate framebuffer and bind renderbuffer just created
		{
			glGenFramebuffers(1, &m_framebufferID);
			glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_bufferID);
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				return false;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}
	else if (m_bufferType == BufferType::Texture)
	{
		// Generate texture
		{
			glGenTextures(1, (GLuint*)&m_bufferID);
			glBindTexture(GL_TEXTURE_2D, m_bufferID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		// Generate framebuffer and bind texture just created
		{
			glGenFramebuffers(1, (GLuint*)&m_framebufferID);
			glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_bufferID, 0);
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	return true;
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
	glViewport(0, 0, m_width, m_height);
}

unsigned int Framebuffer::GetFramebuffer()
{
	return m_framebufferID;
}

unsigned int Framebuffer::GetTexture(bool share)
{
	if (m_bufferType == BufferType::Renderbuffer)
		return 0;

	if (share == true)
		m_bufferShared = true;

	return m_bufferID;
}

unsigned int Framebuffer::GenerateCopiedTexture()
{
	unsigned int texture;

	if (m_bufferType == BufferType::Renderbuffer)
	{
		Framebuffer* textureFramebuffer = new Framebuffer(m_width, m_height, m_sampleCount, Framebuffer::BufferType::Texture);

		if (textureFramebuffer->Initialize() == false)
			return 0;

		glBindFramebuffer(GL_READ_FRAMEBUFFER, GetFramebuffer());
		glReadBuffer(GL_COLOR_ATTACHMENT0);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, textureFramebuffer->GetFramebuffer());
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		textureFramebuffer->DumpBuffer("C:\\dumpbuffer\\whilecopying.ppm");

		texture = textureFramebuffer->GetTexture(true);

		//if (textureFramebuffer != nullptr)
		//{
		//	delete textureFramebuffer;
		//	textureFramebuffer = nullptr;
		//}
	}
	else
	{
		Bind();

		glGenTextures(1, (GLuint*)&texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, m_width, m_height);
	}

	return texture;
}

int Framebuffer::GetWidth()
{
	return m_width;
}

int Framebuffer::GetHeight()
{
	return m_height;
}

Framebuffer::BufferType Framebuffer::GetBufferType()
{
	return m_bufferType;
}

void Framebuffer::DumpBuffer(const char* _filePath, bool _formatP6)
{
	Bind();

	unsigned char *pixels = (unsigned char*)malloc(sizeof(unsigned char)*m_width*m_height * 4);


	if (m_bufferType == BufferType::Renderbuffer)
	{
		// 이상하게 Renderbuffer 자체를 타겟으로 glReadPixels하면 제대로 안나온다...
		// Todo.
		Framebuffer* textureFramebuffer = new Framebuffer(m_width, m_height, m_sampleCount, Framebuffer::BufferType::Texture);
		textureFramebuffer->Initialize();

		glBindFramebuffer(GL_READ_FRAMEBUFFER, GetFramebuffer());
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, textureFramebuffer->GetFramebuffer());
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, textureFramebuffer->GetFramebuffer());
		glReadBuffer(GL_COLOR_ATTACHMENT0);

		glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (textureFramebuffer != nullptr)
		{
			delete textureFramebuffer;
			textureFramebuffer = nullptr;
		}
	}
	else
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0);

		// RGB만 필요해도 왠만하면 일단 RGBA로 읽어오는 것이 안전하다... 이상한 구현체.
		glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	}

	if (_formatP6)
	{
		unsigned char *flipped = (unsigned char*)malloc(sizeof(unsigned char)*m_width*m_height * 3);
		int flippedIdx = 0;
		for (int i = m_height - 1; i >= 0; i--)
		{
			for (int j = 0; j < m_width; j++)
			{
				int pixelIdx = (i * m_width + j) * 4;
				flipped[flippedIdx++] = pixels[pixelIdx];
				flipped[flippedIdx++] = pixels[pixelIdx + 1];
				flipped[flippedIdx++] = pixels[pixelIdx + 2];
			}
		}

		if (_filePath == nullptr)
			_filePath = "../FrontBufferP6.ppm";

		std::ofstream outfile(_filePath, std::ofstream::binary);
		if (outfile.fail())
		{
			free(pixels);
			free(flipped);
			return;
		}

		outfile << "P6\n" << m_width << " " << m_height << "\n" << "255\n";
		outfile.write(reinterpret_cast<char*>(flipped), m_width * m_height * 3);
		outfile.close();

		free(flipped);
	}
	else
	{
		if (_filePath == nullptr)
			_filePath = "../FrontBufferP3.ppm";

		FILE *fp;
		fp = fopen(_filePath, "wt");
		fprintf(fp, "P3\n");
		fprintf(fp, "%d %d\n", m_width, m_height);
		fprintf(fp, "255\n");

		for (int i = m_height - 1; i >= 0; i--)
		{
			for (int j = 0; j < m_width; j++)
			{
				int idx = (i * m_width + j) * 3;
				fprintf(fp, "%u %u %u ",
					(unsigned int)pixels[idx],
					(unsigned int)pixels[idx + 1],
					(unsigned int)pixels[idx + 2]);
			}
		}
		fclose(fp);
	}

	free(pixels);
}