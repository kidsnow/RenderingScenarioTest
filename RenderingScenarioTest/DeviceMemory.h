#pragma once

typedef unsigned int GLenum;
typedef unsigned int GLuint;

class DeviceMemory
{
public:
	enum class MemoryType
	{
		Renderbuffer,
		Texture
	};

	class InternalFormat
	{
	public:
		InternalFormat(GLenum _enum);
		~InternalFormat();

	public:
		static const InternalFormat RGBA_Float8;
		static const InternalFormat RGBA_Float32;
		static const InternalFormat Depth_Float24_Stencil_Int8;

	public:
		explicit operator GLenum() const;
		bool operator==(const InternalFormat&) const;

	private:
		GLenum m_enum;
	};

public:
	static DeviceMemory* GenRenderbuffer(int _width, int _height, int _sampleCount, InternalFormat _format);
	static DeviceMemory* GenTexture(int _width, int _height, InternalFormat _internalFormat);

private:
	DeviceMemory(MemoryType _memoryType, InternalFormat _internalFormat, GLuint _id);
	~DeviceMemory();

public:
	MemoryType GetType();
	InternalFormat GetInternalFormat();
	GLuint GetId();

private:
	MemoryType m_type;
	InternalFormat m_internalFormat;
	GLuint m_bufferId;
};