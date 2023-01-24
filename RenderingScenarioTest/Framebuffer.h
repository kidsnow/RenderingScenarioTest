#pragma once

int CheckGLError(const char* _file, int _line);
#define CHECK_GL_ERROR CheckGLError(__FILE__, __LINE__);

class Framebuffer
{
	class BufferAttachment
	{
	public:
		class Color
		{
		public:
			explicit Color(unsigned int _attachment) : m_attachment(_attachment) {}
			~Color() {}

		private:
			unsigned int m_attachment;
		};

		static const BufferAttachment Depth;
		static const BufferAttachment Stencil;
		static const BufferAttachment DepthStencil;

	private:
		constexpr explicit BufferAttachment(Color color);
		constexpr explicit BufferAttachment(unsigned int _attachment) : m_attachment(_attachment) {}

	private:
		unsigned int m_attachment;
	};

public:
	Framebuffer(int _width, int _height, int _sampleCount);
	~Framebuffer();

	virtual bool Initialize();
	void Bind();
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