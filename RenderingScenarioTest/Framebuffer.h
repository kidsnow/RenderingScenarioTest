#pragma once

class Framebuffer
{
public:
	enum class BufferType
	{
		Renderbuffer,
		Texture
	};

public:
	Framebuffer(int _width, int _height, int _sampleCount, BufferType _bufferType = BufferType::Renderbuffer);
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
	unsigned int m_bufferID;
	bool m_bufferShared;
	unsigned int m_framebufferID;
};