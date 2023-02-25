#include "DeviceMemory.h"

#include "GL/glew.h"

const DeviceMemory::InternalFormat DeviceMemory::InternalFormat::RGBA_Float8(GL_RGBA8);
const DeviceMemory::InternalFormat DeviceMemory::InternalFormat::RGBA_Float32(GL_RGBA32F);
const DeviceMemory::InternalFormat DeviceMemory::InternalFormat::Depth_Float24_Stencil_Int8(GL_DEPTH24_STENCIL8);
DeviceMemory::InternalFormat::InternalFormat(GLenum _enum) : m_enum(_enum) {}
DeviceMemory::InternalFormat::~InternalFormat() {}

DeviceMemory::InternalFormat::operator GLenum() const
{
	return m_enum;
}

bool DeviceMemory::InternalFormat::operator==(const InternalFormat& _internalFormat) const
{
	return GLenum(*this) == GLenum(_internalFormat);
}

DeviceMemory::DeviceMemory(MemoryType _memoryType, InternalFormat _internalFormat, GLuint _id) :
	m_type(_memoryType),
	m_internalFormat(_internalFormat),
	m_bufferId(_id)
{
}

DeviceMemory::~DeviceMemory()
{
}

DeviceMemory* DeviceMemory::GenRenderbuffer(int _width, int _height, int _sampleCount, InternalFormat _internalFormat)
{
	// FlushGLError하고

	GLenum internalFormat = GLenum(_internalFormat);

	GLuint bufferId = 0;
	glGenRenderbuffers(1, &bufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, bufferId);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, _sampleCount, internalFormat, _width, _height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	DeviceMemory* deviceMemory = nullptr;
	deviceMemory = new DeviceMemory(MemoryType::Renderbuffer, _internalFormat, bufferId);

	// CHECK_GL_ERROR해서 문제되면 nullptr 반환.
	return deviceMemory;
}

DeviceMemory* DeviceMemory::GenTexture(int _width, int _height, InternalFormat _internalFormat)
{
	GLenum textureInternalFormat = GL_NONE;
	GLenum texturePixelFormat = GL_NONE;
	GLenum texturePixelDataType = GL_NONE;

	if (_internalFormat == DeviceMemory::InternalFormat::RGBA_Float8)
	{
		textureInternalFormat = GLenum(_internalFormat);
		texturePixelFormat = GL_RGBA;
		texturePixelDataType = GL_UNSIGNED_BYTE;
	}
	else
	{
		// Not supported.
		return nullptr;
	}

	GLuint bufferId = 0;
	glGenTextures(1, &bufferId);
	glBindTexture(GL_TEXTURE_2D, bufferId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return nullptr;
}

DeviceMemory::MemoryType DeviceMemory::GetType()
{
	return m_type;
}

DeviceMemory::InternalFormat DeviceMemory::GetInternalFormat()
{
	return m_internalFormat;
}

GLuint DeviceMemory::GetId()
{
	return m_bufferId;
}