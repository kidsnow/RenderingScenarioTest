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
		static const InternalFormat R_Float32;
		static const InternalFormat RGBA_Float8;
		static const InternalFormat RGBA_Float32;
		static const InternalFormat Depth_Float24_Stencil_Int8;

	public:
		explicit operator GLenum() const;
		bool operator==(const InternalFormat&) const;
		bool operator!=(const InternalFormat&) const;

	private:
		GLenum m_enum;
	};

private:
	DeviceMemory(MemoryType _memoryType, InternalFormat _internalFormat, GLuint _id);

public:
	~DeviceMemory();
	static DeviceMemory* GenRenderbuffer(int _width, int _height, int _sampleCount, InternalFormat _format);
	static DeviceMemory* GenTexture(int _width, int _height, InternalFormat _internalFormat);

public:
	// Mark that this instance's lifecycle is managed by other object from RenderSystem.
	void SetManaged(bool _managed);
	// If this returns true, this instance is managed by other object from RenderSystem so don't allocate new device memory or free this memory.
	bool IsManaged();
	MemoryType GetType();
	InternalFormat GetInternalFormat();
	GLuint GetId();

private:
	bool m_managed; // managed by rendersystem. no need to manually free memory.
	MemoryType m_type;
	InternalFormat m_internalFormat;
	GLuint m_bufferId;
};