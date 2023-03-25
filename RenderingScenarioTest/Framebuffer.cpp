#include "Framebuffer.h"

#include "GL/glew.h"

#include <vector>
#include <fstream>

#include "DeviceMemory.h"
#include "GLHelper.h"

//const Attachment Attachment::Depth(GL_DEPTH_ATTACHMENT);
//const Attachment Attachment::Stencil(GL_STENCIL_ATTACHMENT);
const Attachment Attachment::DepthStencil(GL_DEPTH_STENCIL_ATTACHMENT);
Attachment::Color::Color(unsigned int _index) :
	m_index(_index) {}
Attachment::Color::~Color() {}

Attachment::Color::operator GLenum() const
{
	return GL_COLOR_ATTACHMENT0 + m_index;
}

Attachment::Attachment(Color _color) :
	m_attachment(GLenum(_color))
{

}

Attachment::Attachment(GLenum _attachment) :
	m_attachment(_attachment)
{

}

Attachment::operator GLenum() const
{
	return m_attachment;
}

bool Attachment::operator==(const Attachment& _attachment) const
{
	return GLenum(*this) == GLenum(_attachment);
}

bool Attachment::operator<(const Attachment& _attachment) const
{
	return this->m_attachment < _attachment.m_attachment;
}

bool Attachment::operator>(const Attachment& _attachment) const
{
	return this->m_attachment > _attachment.m_attachment;
}

bool Attachment::IsColorAttachment() const
{
	int maxColorAttachmentCount;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachmentCount);

	GLenum maxColorAttachment = GL_COLOR_ATTACHMENT0 + maxColorAttachmentCount;

	if (m_attachment >= GL_COLOR_ATTACHMENT0 &&
		m_attachment < GL_COLOR_ATTACHMENT0 + maxColorAttachment)
	{
		return true;
	}

	return false;
}

// Temp constructor for managing default framebuffer. Will be deleted soon.
Framebuffer::Framebuffer(int _width, int _height) :
	m_width(_width),
	m_height(_height),
	m_id(0)
{
	// Currently, CLO's default framebuffer ID is 0.
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

	for (auto managedBuffer : m_managedBuffers)
	{
		if (managedBuffer->IsManaged())
			delete managedBuffer;
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

	glViewport(0, 0, m_width, m_height);

	if (m_attachedBuffers.empty())
		return;

	std::vector<GLenum> attachments;
	for (auto attachedBuffer : m_attachedBuffers)
	{
		auto attachment = attachedBuffer.first;
		if (attachment.IsColorAttachment())
		{
			attachments.push_back(GLenum(attachment));
		}
	}

	glDrawBuffers(static_cast<int>(attachments.size()), attachments.data());
}

void Framebuffer::AddRenderbuffer(DeviceMemory::InternalFormat _internalFormat, Attachment _attachment)
{
	auto renderbuffer = DeviceMemory::GenRenderbuffer(m_width, m_height, m_sampleCount, _internalFormat);

	if (renderbuffer == nullptr)
		return;

	registerManagedBuffer(renderbuffer);

	AttachRenderbuffer(renderbuffer, _attachment);

	return;
}

void Framebuffer::AddTexture(DeviceMemory::InternalFormat _internalFormat, Attachment _attachment)
{
	if (_attachment.IsColorAttachment() == false)
		return;

	DeviceMemory::InternalFormat internalFormat = DeviceMemory::InternalFormat::RGBA_Float8;

	auto texture = DeviceMemory::GenTexture(m_width, m_height, internalFormat);

	if (texture == nullptr)
		return;

	registerManagedBuffer(texture);

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

void Framebuffer::registerManagedBuffer(DeviceMemory* _deviceMemory)
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
		return "DepthStencilAttachment";
	}

	return "UnknownAttachment";
}

DeviceMemory* Framebuffer::GenerateBlittedTexture(Attachment _attachment)
{
	auto deviceMemory = m_attachedBuffers[_attachment];

	bool supportedFormat = false;
	if (_attachment.IsColorAttachment())
	{
		supportedFormat = true;
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
}

void Framebuffer::dumpTexture(DeviceMemory* _texture, const char* _fileName, bool _formatP6)
{
	auto textureFramebuffer = new Framebuffer(m_width, m_height, m_sampleCount);
	textureFramebuffer->AttachTexture(_texture, Attachment::Color(0));

	auto internalFormat = _texture->GetInternalFormat();
	if (internalFormat == DeviceMemory::InternalFormat::RGBA_Float8)
	{
		unsigned char* pixels = (unsigned char*)malloc(sizeof(unsigned char) * m_width * m_height * 4);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, textureFramebuffer->GetId());
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		// RGB만 필요해도 왠만하면 일단 RGBA로 읽어오는 것이 안전하다... 이상한 구현체.
		glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

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
	else if (internalFormat == DeviceMemory::InternalFormat::R_Float32)
	{
		float* pixels = (float*)malloc(sizeof(float) * m_width * m_height);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, textureFramebuffer->GetId());
		glReadBuffer(GL_COLOR_ATTACHMENT0);

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(0, 0, m_width, m_height, GL_RED, GL_FLOAT, pixels);

		if (false)
		{
			unsigned char* flipped = (unsigned char*)malloc(sizeof(unsigned char) * m_width * m_height * 3);
			int flippedIdx = 0;
			for (int i = m_height - 1; i >= 0; i--)
			{
				for (int j = 0; j < m_width; j++)
				{
					int pixelIdx = (i * m_width + j);
					flipped[flippedIdx++] = pixels[pixelIdx] * 255.0f;
					flipped[flippedIdx++] = pixels[pixelIdx] * 255.0f;
					flipped[flippedIdx++] = pixels[pixelIdx] * 255.0f;
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
					int idx = i * m_width + j;
					/*fprintf(fp, "%u %u %u ",
						(unsigned int)(pixels[idx] * 255.0f),
						(unsigned int)(pixels[idx] * 255.0f),
						(unsigned int)(pixels[idx] * 255.0f));*/
					fprintf(fp, "%f %f %f ",
						(pixels[idx] * 255.0f),
						(pixels[idx] * 255.0f),
						(pixels[idx] * 255.0f));
				}
			}
			fclose(fp);
		}

		free(pixels);
	}

	delete textureFramebuffer;
}