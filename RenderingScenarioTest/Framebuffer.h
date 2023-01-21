#pragma once

int CheckGLError(const char* _file, int _line);
#define CHECK_GL_ERROR CheckGLError(__FILE__, __LINE__);

class Framebuffer
{
public:
	enum class BufferType
	{
		Renderbuffer,
		Texture
	};

public:
	Framebuffer(int _width, int _height, int _sampleCount, int _colorAttachmentCount, bool _depthStencilAttachment, BufferType _bufferType);
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
	BufferType m_bufferType;
	unsigned int* m_colorBuffers;
	unsigned int m_depthBuffer;
	bool m_bufferShared;
	unsigned int m_framebufferID;
};