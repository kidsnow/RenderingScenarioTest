#pragma once

#include <map>
#include <string>

/*
 * Framebuffer.h
 * FBO 생성 및 관리.
 * FBO를 구성하기 위해 붙는 device memory에 대한 관리.
 * 일단 Framebuffer class의 instance를 위해 생성되고 관리되는 attachment들은 외부에서 사용하지 못하도록 할 예정.
 * 외부에서 특정 attachment가 필요하다면 복사하는게 관리하기에는 일단 더 편하지 않을까 싶음.
 */

int CheckGLError(const char* _file, int _line);
#define CHECK_GL_ERROR CheckGLError(__FILE__, __LINE__);

typedef unsigned int GLenum;
typedef unsigned int GLuint;
class DeviceMemory;

class Framebuffer
{
public:
	class Attachment
	{
	friend Framebuffer;
	public:
		class Color
		{
		public:
			explicit Color(unsigned int _attachment);
			~Color();

		public:
			explicit operator GLenum() const;

		private:
			unsigned int m_attachment;
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

public:
	Framebuffer(int _width, int _height, int _sampleCount);
	~Framebuffer();

	unsigned int GetId();
	int GetWidth();
	int GetHeight();

	void Bind();

	Framebuffer& AddRenderbuffer(Attachment _attachment);
	Framebuffer& AddTexture(Attachment _attachment);

	bool IsComplete();

	GLuint GenerateBlittedTexture(Attachment _attachment);
	void DumpAllAttachments(const char* _filePath, bool _formatP6 = true);

private:
	std::string getDumpedImageName(Attachment _attachment);

private:
	int m_width, m_height;
	int m_sampleCount;
	unsigned int m_id;

	std::map <Attachment, DeviceMemory*> m_attachments;
};