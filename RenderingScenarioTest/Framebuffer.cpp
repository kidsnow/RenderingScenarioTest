#include "Framebuffer.h"

#include "GL/glew.h"
#include <fstream>

// GL ERROR CHECK
int CheckGLError(const char* _file, int _line)
{
	int    retCode = 0;

#if !defined(NDEBUG) || defined(_DEBUG) // define 없애지 말것 (릴리즈에서는 그릴때마다 glError체크하면 속도저하생김)
	GLenum glErr;
	glErr = glGetError();
	while (glErr != GL_NO_ERROR)
	{
		const char* sError = nullptr;

		switch (glErr)
		{
		case GL_NO_ERROR:
			sError = "GL_NO_ERROR";
			break;
		case GL_INVALID_ENUM:
			sError = "GL_INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			sError = "GL_INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			sError = "GL_INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			sError = "GL_STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			sError = "GL_STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			sError = "GL_OUT_OF_MEMORY";
			break;
		case GL_TABLE_TOO_LARGE:
			sError = "GL_TABLE_TOO_LARGE";
			break;
		}

		if (sError)
			printf("GL Error #%d (%s) in File %s at line: %d \n", glErr, sError, _file, _line);
		else
			printf("GL Error #%d (no message available) in File %s at line: %d \n", glErr, _file, _line);

		if (glErr == GL_STACK_UNDERFLOW || glErr == GL_STACK_OVERFLOW)
		{
			GLint matrixMode;
			GLint stackDepth;
			GLint activeTexture;

			glGetIntegerv(GL_MATRIX_MODE, &matrixMode);

			switch (matrixMode)
			{
			case GL_MODELVIEW:
				glGetIntegerv(GL_MODELVIEW_STACK_DEPTH, &stackDepth);
				printf("GL_MODELVIEW, stack(%d) \n", stackDepth);
				break;
			case GL_PROJECTION:
				glGetIntegerv(GL_PROJECTION_STACK_DEPTH, &stackDepth);
				printf("GL_PROJECTION, stack(%d) \n", stackDepth);
				break;
			case GL_TEXTURE:
				glGetIntegerv(GL_ACTIVE_TEXTURE, &activeTexture);
				glGetIntegerv(GL_TEXTURE_STACK_DEPTH, &stackDepth);
				printf("GL_TEXTURE, stack(%d) \n", stackDepth);
				break;
			}
		}

		retCode = 1;
		glErr = glGetError();
	}
#endif
	return retCode;
}

Framebuffer::Framebuffer(int _width, int _height, int _sampleCount, BufferType _bufferType) :
	m_width(_width),
	m_height(_height),
	m_bufferType(_bufferType),
	m_sampleCount(_sampleCount),
	m_colorBuffer(0),
	m_depthBuffer(0),
	m_bufferShared(false),
	m_framebufferID(0)
{
}

Framebuffer::~Framebuffer()
{
	if (m_colorBuffer != 0)
	{
		if (m_bufferType == BufferType::Renderbuffer)
		{
			glDeleteRenderbuffers(1, (GLuint*)&m_colorBuffer);
		}
		else if (m_bufferType == BufferType::Texture)
		{
			if (m_bufferShared == false)
			{
				glDeleteTextures(1, (GLuint*)&m_colorBuffer);
			}
		}
		m_colorBuffer = 0;
	}

	if (m_depthBuffer != 0)
	{
		if (m_bufferType == BufferType::Renderbuffer)
		{
			glDeleteRenderbuffers(1, (GLuint*)&m_depthBuffer);
		}
		m_depthBuffer = 0;
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
			glGenRenderbuffers(1, &m_colorBuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, m_colorBuffer);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_sampleCount, GL_RGBA8, m_width, m_height);
			glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_sampleCount, GL_RGBA8, m_width, m_height);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}

		// Generate framebuffer and bind renderbuffer just created
		{
			glGenFramebuffers(1, &m_framebufferID);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebufferID);
			glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorBuffer);
			if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				return false;
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		}

		CHECK_GL_ERROR
	}
	else if (m_bufferType == BufferType::Texture)
	{
		// Generate texture
		{
			glGenTextures(1, (GLuint*)&m_colorBuffer);
			glBindTexture(GL_TEXTURE_2D, m_colorBuffer);
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
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer, 0);
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		CHECK_GL_ERROR
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

	return m_colorBuffer;
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
	CHECK_GL_ERROR;
	Bind();

	CHECK_GL_ERROR;
	unsigned char *pixels = (unsigned char*)malloc(sizeof(unsigned char)*m_width*m_height * 4);

	CHECK_GL_ERROR;

	if (m_bufferType == BufferType::Renderbuffer)
	{
		// 이상하게 Renderbuffer 자체를 타겟으로 glReadPixels하면 제대로 안나온다...
		// Todo.
		Framebuffer* textureFramebuffer = new Framebuffer(m_width, m_height, m_sampleCount, Framebuffer::BufferType::Texture);
		textureFramebuffer->Initialize();

		CHECK_GL_ERROR;
		glBindFramebuffer(GL_READ_FRAMEBUFFER, GetFramebuffer());
		CHECK_GL_ERROR;
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		CHECK_GL_ERROR;
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, textureFramebuffer->GetFramebuffer());
		CHECK_GL_ERROR;
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		CHECK_GL_ERROR;
		glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		CHECK_GL_ERROR;

		textureFramebuffer->DumpBuffer(_filePath);

		if (textureFramebuffer != nullptr)
		{
			delete textureFramebuffer;
			textureFramebuffer = nullptr;
		}

		return;
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