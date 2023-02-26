#include "Framebuffer.h"

#include "GL/glew.h"

#include <vector>
#include <fstream>

#include "DeviceMemory.h"

//const Framebuffer::Attachment Framebuffer::Attachment::Depth(GL_DEPTH_ATTACHMENT);
//const Framebuffer::Attachment Framebuffer::Attachment::Stencil(GL_STENCIL_ATTACHMENT);
const Framebuffer::Attachment Framebuffer::Attachment::DepthStencil(GL_DEPTH_STENCIL_ATTACHMENT);
Framebuffer::Attachment::Color::Color(unsigned int _attachment) :
	m_attachment(GL_COLOR_ATTACHMENT0 + _attachment) {}
Framebuffer::Attachment::Color::~Color() {}

Framebuffer::Attachment::Color::operator GLenum() const
{
	return m_attachment;
}

Framebuffer::Attachment::Attachment(Color _color) :
	m_attachment(GLenum(_color))
{

}

Framebuffer::Attachment::Attachment(GLenum _attachment) :
	m_attachment(_attachment)
{

}

Framebuffer::Attachment::operator GLenum() const
{
	return m_attachment;
}

bool Framebuffer::Attachment::operator==(const Attachment& _attachment) const
{
	return GLenum(*this) == GLenum(_attachment);
}

bool Framebuffer::Attachment::operator<(const Attachment& _attachment) const
{
	return this->m_attachment < _attachment.m_attachment;
}

bool Framebuffer::Attachment::operator>(const Attachment& _attachment) const
{
	return this->m_attachment > _attachment.m_attachment;
}

bool Framebuffer::Attachment::IsColorAttachment() const
{
	int maxColorAttachmentCount;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachmentCount);

	if (m_attachment >= GL_COLOR_ATTACHMENT0 &&
		m_attachment < GL_COLOR_ATTACHMENT0 + maxColorAttachmentCount)
	{
		return true;
	}

	return false;
}

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

Framebuffer::Framebuffer(int _width, int _height, int _sampleCount) :
	m_width(_width),
	m_height(_height),
	m_sampleCount(_sampleCount),
	m_id(0)
{
	glGenFramebuffers(1, &m_id);
}

Framebuffer::~Framebuffer()
{
	if (m_id != 0)
	{
		glDeleteFramebuffers(1, (GLuint*)&m_id);
		m_id = 0;
	}

	for (auto attachedBuffer : m_attachedBuffers)
	{
		auto deviceMemory = attachedBuffer.second;

		if (deviceMemory->IsManaged())
			delete deviceMemory;
	}
}

unsigned int Framebuffer::GetId()
{
	return m_id;
}

int Framebuffer::GetWidth()
{
	return m_width;
}

int Framebuffer::GetHeight()
{
	return m_height;
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);

	std::vector<GLenum> attachments;
	for (auto attachedBuffer : m_attachedBuffers)
	{
		auto attachment = attachedBuffer.first;
		if (attachment.IsColorAttachment())
		{
			attachments.push_back(GLenum(attachment));
		}
	}

	glDrawBuffers(attachments.size(), attachments.data());
	glViewport(0, 0, m_width, m_height);
}

void Framebuffer::AddRenderbuffer(Attachment _attachment)
{
	DeviceMemory::InternalFormat internalFormat = DeviceMemory::InternalFormat::RGBA_Float8;
	if (_attachment == Attachment::DepthStencil)
	{
		internalFormat = DeviceMemory::InternalFormat::Depth_Float24_Stencil_Int8;
	}

	auto renderbuffer = DeviceMemory::GenRenderbuffer(m_width, m_height, m_sampleCount, internalFormat);

	if (renderbuffer == nullptr)
		return;

	RegisterManagedBuffer(renderbuffer);

	AttachRenderbuffer(renderbuffer, _attachment);

	return;
}

void Framebuffer::AddTexture(Attachment _attachment)
{
	if (_attachment.IsColorAttachment() == false)
		return;

	DeviceMemory::InternalFormat internalFormat = DeviceMemory::InternalFormat::RGBA_Float8;

	auto texture = DeviceMemory::GenTexture(m_width, m_height, internalFormat);

	if (texture == nullptr)
		return;

	RegisterManagedBuffer(texture);

	AttachTexture(texture, _attachment);

	return;
}

void Framebuffer::AttachRenderbuffer(DeviceMemory* _renderbuffer, Attachment _attachment)
{
	if (_renderbuffer->GetType() != DeviceMemory::MemoryType::Renderbuffer)
		return;

	auto renderbufferId = _renderbuffer->GetId();

	// Attach renderbuffer to framebuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_id);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GLenum(_attachment), GL_RENDERBUFFER, renderbufferId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	m_attachedBuffers[_attachment] = _renderbuffer;

	return;
}

void Framebuffer::AttachTexture(DeviceMemory* _texture, Attachment _attachment)
{
	if (_texture->GetType() != DeviceMemory::MemoryType::Texture)
		return;

	auto textureId = _texture->GetId();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_id);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GLenum(_attachment), GL_TEXTURE_2D, textureId, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	m_attachedBuffers[_attachment] = _texture;

	return;
}

void Framebuffer::RegisterManagedBuffer(DeviceMemory* _deviceMemory)
{
	m_managedBuffers.push_back(_deviceMemory);
	_deviceMemory->SetManaged(true);
}

bool Framebuffer::IsComplete()
{
	if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	return true;
}

std::string Framebuffer::getDumpedImageName(Attachment _attachment)
{
	if (_attachment.IsColorAttachment())
	{
		int index = GLenum(_attachment) - GL_COLOR_ATTACHMENT0;
		return "ColorAttachment" + std::to_string(index);
	}
	else if (_attachment == Attachment::DepthStencil)
	{
		return "DepthStencil";
	}
}

DeviceMemory* Framebuffer::GenerateBlittedTexture(Attachment _attachment)
{
	auto deviceMemory = m_attachedBuffers[_attachment];

	bool supportedFormat = false;
	if (_attachment.IsColorAttachment())
	{
		if (deviceMemory->GetType() == DeviceMemory::MemoryType::Renderbuffer)
		{
			supportedFormat = true;
		}
	}

	if (supportedFormat == false)
		return nullptr;

	auto texture = DeviceMemory::GenTexture(m_width, m_height, deviceMemory->GetInternalFormat());
	
	if (texture == nullptr)
		return nullptr;

	auto textureFramebuffer = new Framebuffer(m_width, m_height, m_sampleCount);
	textureFramebuffer->AttachTexture(texture, Attachment::Color(0));

	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id);
	glReadBuffer(GLenum(_attachment));
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, textureFramebuffer->GetId());
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	delete textureFramebuffer;

	return texture;
}

void Framebuffer::DumpAllAttachments(const char* _filePath, bool _formatP6)
{
	for (auto attachedBuffer : m_attachedBuffers)
	{
		auto attachment = attachedBuffer.first;

		auto texture = GenerateBlittedTexture(attachment);

		if (texture == nullptr)
			continue;

		std::string fileName = std::string(_filePath) + std::string("\\" + getDumpedImageName(attachment) + std::string(".ppm"));
		dumpTexture(texture, fileName.c_str());

		delete texture;
	}

	CHECK_GL_ERROR;
}

void Framebuffer::dumpTexture(DeviceMemory* _texture, const char* _fileName, bool _formatP6)
{
	auto textureFramebuffer = new Framebuffer(m_width, m_height, m_sampleCount);
	textureFramebuffer->AttachTexture(_texture, Attachment::Color(0));

	{
		unsigned char* pixels = (unsigned char*)malloc(sizeof(unsigned char) * m_width * m_height * 4);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, textureFramebuffer->GetId());
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		// RGB만 필요해도 왠만하면 일단 RGBA로 읽어오는 것이 안전하다... 이상한 구현체.
		glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		CHECK_GL_ERROR;
		if (_formatP6)
		{
			unsigned char* flipped = (unsigned char*)malloc(sizeof(unsigned char) * m_width * m_height * 3);
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

			std::ofstream outfile(_fileName, std::ofstream::binary);
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
			FILE* fp;
			fp = fopen(_fileName, "wt");
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

	delete textureFramebuffer;
}