#pragma once

#include <map>
#include <vector>
#include <string>

#include "DeviceMemory.h"

/*
 * Framebuffer.h
 * FBO 생성 및 관리.
 * FBO를 구성하기 위해 붙는 device memory에 대한 관리.
 * 일단 Framebuffer class의 instance를 위해 생성되고 관리되는 attachment들은 외부에서 사용하지 못하도록 할 예정.
 * 외부에서 특정 attachment가 필요하다면 복사하는게 관리하기에는 일단 더 편하지 않을까 싶음.
 */

typedef unsigned int GLenum;
typedef unsigned int GLuint;
class DeviceMemory;

class Attachment
{
public:
	class Color
	{
	public:
		explicit Color(unsigned int _index);
		~Color();

	public:
		explicit operator GLenum() const;

	private:
		unsigned int m_index;
	};

	//static const Attachment Depth;
	//static const Attachment Stencil;
	static const Attachment DepthStencil;

public:
	Attachment(Color _color);
	explicit Attachment(GLenum _attachment);

public:
	explicit operator GLenum() const;
	bool operator==(const Attachment&) const;
	bool operator<(const Attachment&) const;
	bool operator>(const Attachment&) const;
	bool IsColorAttachment() const;

private:
	GLenum m_attachment;
};

class Framebuffer
{
public:
	Framebuffer(int _width, int _height); // Temp constructor for managing default framebuffer. Will be deleted soon.
	Framebuffer(int _width, int _height, int _sampleCount);
	~Framebuffer();

	unsigned int GetId();
	int GetWidth();
	int GetHeight();

	// Bind framebuffer and every attachments to draw buffer.
	void Bind();

	// Add[Renderbuffer/Texture] function:
	//     Allocate device memory and attach it to this framebuffer.
	//     The lifecycle of the input device memory will be handled by this object.
	void AddRenderbuffer(DeviceMemory::InternalFormat _internalFormat, Attachment _attachment);
	void AddTexture(DeviceMemory::InternalFormat _internalFormat, Attachment _attachment);

	// Attach[Renderbuffer/Texture] function:
	//     Attaches incoming device memory to this framebuffer.
	//     The input device memory is not managed by this object.
	void AttachRenderbuffer(DeviceMemory* _renderbuffer, Attachment _attachment);
	void AttachTexture(DeviceMemory* _texture, Attachment _attachment);

	// It's recommended to validate the frambuffer object with this function before binding to the rendering logic.
	// At least you can make sure you're not missing any state that is essential for rendering.
	bool IsComplete();

	// The texture returned by this function is not managed by its framebuffer object;
	// the lifecycle of that texture does not end when the framebuffer is deleted.
	DeviceMemory* GenerateBlittedTexture(Attachment _attachment);
	// If _formatP6 is true, dump the image file as binary, otherwise dump it in ascii format.
	void DumpAllAttachments(const char* _filePath, bool _formatP6 = true);

private:
	// Register input DeviceMemory instance to this object in order to manage lifecycle.
	void registerManagedBuffer(DeviceMemory* _deviceMemory);
	void dumpTexture(DeviceMemory* _texture, const char* _fileName, bool _formatP6 = true);
	std::string getDumpedImageName(Attachment _attachment);

private:
	int m_width, m_height;
	int m_sampleCount;
	unsigned int m_id;

	// DeviceMemory instances which are attached to this framebuffer.
	std::map <Attachment, DeviceMemory*> m_attachedBuffers;

	// Registered DeviceMemory instances to manage lifecycle.
	// These buffers will be freed when this object is destroyed.
	std::vector<DeviceMemory*> m_managedBuffers;
};