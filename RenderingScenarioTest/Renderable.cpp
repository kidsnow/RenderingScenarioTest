#include "Renderable.h"
#include "Texture.h"
#include "Framebuffer.h"
#include "ShaderManager.h"

TextureForBlending::TextureForBlending(int width, int height) :
	_width(width),
	_height(height),
	_target(0),
	_source(1)
{
	_texture[_target] = new Texture(width, height);
	_texture[_source] = new Texture(width, height);
}

TextureForBlending::~TextureForBlending()
{
	delete _texture[0];
	delete _texture[1];
}

void TextureForBlending::SwapTextures()
{
	_target = (_target + 1) % 2;
	_source = (_source + 1) % 2;
}

Renderable::Renderable(glm::vec2 size) :
	_size(size),
	_vao(0),
	_vertices(nullptr),
	_texture(nullptr),
	_textureForBlending(nullptr),
	_normalBlendMode(DIFFERENTIAL)
{
	initializeVAO();
}

Renderable::~Renderable()
{
	glDeleteVertexArrays(1, &_vao);
	if (_textureForBlending != nullptr)
		delete _textureForBlending;
}

void Renderable::initializeVAO()
{
	_vertices = new GLfloat[30]
	{
		-_size.x / 2.0f, -_size.y / 2.0f, 0.0f, 0.0f, 0.0f,	// lower left
		 _size.x / 2.0f, -_size.y / 2.0f, 0.0f, 1.0f, 0.0f,	// lower right
		-_size.x / 2.0f,  _size.y / 2.0f, 0.0f, 0.0f, 1.0f,	// upper left
		-_size.x / 2.0f,  _size.y / 2.0f, 0.0f, 0.0f, 1.0f,	// upper left
		 _size.x / 2.0f, -_size.y / 2.0f, 0.0f, 1.0f, 0.0f,	// lower right
		 _size.x / 2.0f,  _size.y / 2.0f, 0.0f, 1.0f, 1.0f,	// upper right
	};

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 5, _vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderable::BlendNormalMap(bool dumpFlag)
{
	if (_textureForBlending == nullptr)
	{
		_textureForBlending = new TextureForBlending(_texture->GetWidth(), _texture->GetHeight());
	}

	// 먼저 base texture copy 진행.
	// Target에 base texture copy.
	Framebuffer* framebuffer = new Framebuffer();
	framebuffer->SetRenderTarget(_textureForBlending->GetTargetTexture());
	framebuffer->Bind();

	Shader* shader = ShaderManager::Instance()->GetShader("simple");
	shader->Use();

	glm::mat4 identity(1.0f);
	shader->SetMatrix4("MVP", identity);

	Renderable* forBaseTexture = new Renderable(glm::vec2(2.0, 2.0));
	forBaseTexture->BindTexture(_texture);
	forBaseTexture->Render();


	shader = ShaderManager::Instance()->GetShader("normal_blend");
	shader->Use();
	shader->SetInteger("blendMode", 1);

	_textureForBlending->SwapTextures();
	_textureForBlending->GetSourceTexture();

	shader->SetInteger("baseTexture", 0);
	shader->SetMatrix4("MVP", identity);
	shader->SetInteger("detailTexture", 1);
	_textureForBlending->GetSourceTexture()->SetUnitIndex(0);
	_detailList[0]->GetTexture()->SetUnitIndex(1);
	_textureForBlending->GetSourceTexture()->Render();
	_detailList[0]->GetTexture()->Render();

	framebuffer->SetRenderTarget(_textureForBlending->GetTargetTexture());
	framebuffer->Bind();

	forBaseTexture->BindTexture(nullptr);
	forBaseTexture->Render();

	//for (auto detail : _detailList)
	//{
	//	_textureForBlending->SwapTextures();
	//	shader->SetMatrix4("MVP", identity);
	//	shader->SetInteger("baseTexture", 0);
	//	_textureForBlending->GetSourceTexture()->SetUnitIndex(0);
	//	shader->SetInteger("detailTexture", 1);
	//	detail->GetTexture()->SetUnitIndex(1);
	//
	//	framebuffer->SetRenderTarget(_textureForBlending->GetTargetTexture());
	//	framebuffer->Bind();
	//
	//	_textureForBlending->GetSourceTexture()->Render();
	//	detail->GetTexture()->Render();
	//
	//	forBaseTexture->BindTexture(detail->GetTexture());
	//	forBaseTexture->Render();
	//	//glBindVertexArray(_vao);
	//	//glDrawArrays(GL_TRIANGLES, 0, 6);
	//	//glFinish();
	//}

	if (dumpFlag)
	{
		std::string fileName = "texture" + std::to_string(_textureForBlending->GetTargetTexture()->GetID()) + ".ppm";
		framebuffer->DumpFBO2PPM(fileName.c_str());
	}
	framebuffer->Unbind();
}

void Renderable::Render()
{
	if (_texture != nullptr)
	{
		if (_detailList.empty())
		{
			_texture->Render();
		}
		else
		{
			_textureForBlending->GetTargetTexture()->Render();
		}
	}
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glFinish();
}

void Renderable::BindTexture(Texture* texture)
{
	_texture = texture;
}

void Renderable::AddDetail(Renderable* detail)
{
	_detailList.push_back(detail);
	BlendNormalMap();
}