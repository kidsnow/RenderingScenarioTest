#pragma once

#include <map>

int CheckGLError(const char* _file, int _line);
#define CHECK_GL_ERROR CheckGLError(__FILE__, __LINE__);

typedef unsigned int GLenum;
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

	Framebuffer& AddRenderbuffer(Attachment attachment);
	Framebuffer& AddTexture(Attachment attachment);

	bool IsComplete();

	void DumpBuffer(const char* _filePath = nullptr, bool _formatP6 = true);

private:
	int m_width, m_height;
	int m_sampleCount;
	unsigned int m_id;

	std::map <Attachment, DeviceMemory*> m_attachments;
};