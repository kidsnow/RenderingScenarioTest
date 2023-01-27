#pragma once


int CheckGLError(const char* _file, int _line);
#define CHECK_GL_ERROR CheckGLError(__FILE__, __LINE__);


typedef unsigned int GLenum;
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
			~Color() {}

		private:
			unsigned int m_attachment;
		};

		static const Attachment Depth;
		static const Attachment Stencil;
		static const Attachment DepthStencil;

	public:
		constexpr explicit operator GLenum() const { return m_attachment; }

	private:
		constexpr explicit Attachment(Color color);
		constexpr explicit Attachment(GLenum _attachment) : m_attachment(_attachment) {}

	private:
		GLenum m_attachment;
	};

private:
	class DeviceMemory
	{
	public:
		enum class Type
		{
			Renderbuffer,
			Texture
		};

	public:
		explicit DeviceMemory(Type _type);
		~DeviceMemory();

	public:
		void Attach(GLenum _framebufferID);
		void DumpImage();

	private:
		Type m_type;
	};

public:
	Framebuffer(int _width, int _height, int _sampleCount);
	~Framebuffer();

	virtual bool Initialize();
	void Bind();

	Framebuffer& AddRenderbuffer(Attachment attachment);
	Framebuffer& AddTexture(Attachment attachment);

	bool IsComplete();

	unsigned int GetFramebuffer();
	unsigned int GetTexture(bool share = false);
	unsigned int GenerateCopiedTexture();
	int GetWidth();
	int GetHeight();
	BufferType GetBufferType();
	void DumpBuffer(const char* _filePath = nullptr, bool _formatP6 = true);

private:
	int m_width, m_height;
	int m_sampleCount;
	int m_colorAttachmentCount;
	BufferType m_bufferType;
	unsigned int* m_colorBuffers;
	unsigned int m_depthBuffer;
	bool m_bufferShared;
	unsigned int m_framebufferID;
};