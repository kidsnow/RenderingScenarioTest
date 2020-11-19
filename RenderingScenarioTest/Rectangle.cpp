#include "Rectangle.h"
#include "ShaderManager.h"
#include "Texture.h"
#include "Framebuffer.h"
#include "TextureForBlending.h"
#include "NBDetailRectangle.h"
#include <iostream>

Rectangle::Rectangle(glm::vec2 size) :
	_size(size),
	_textureForBlending(nullptr),
	_normalBlendMode(PARTIAL_DERIVATIVE)
{
	createVertexArray();
}

Rectangle::~Rectangle()
{
	if (_textureForBlending != nullptr)
		delete _textureForBlending;
}

void Rectangle::createVertexArray()
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

	initializeVAO();
}

void Rectangle::Render()
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
	Renderable::Render();
}

void Rectangle::AddDetail(NBDetailType type, Rectangle* detailRectangle)
{
	NBDetailRectangle* detail = new NBDetailRectangle(type, detailRectangle);
	_detailList.push_back(detail);
	BlendNormalMap();
}

void Rectangle::BlendNormalMap(bool dumpFlag)
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

	Rectangle* forBaseTexture = new Rectangle(glm::vec2(2.0, 2.0));
	forBaseTexture->BindTexture(_texture);
	forBaseTexture->Render();


	shader = ShaderManager::Instance()->GetShader("normal_blend");
	shader->Use();
	//shader->SetInteger("blendMode", _normalBlendMode);
	shader->SetInteger("blendMode", _normalBlendMode);

	for (auto detail : _detailList)
	{
		_textureForBlending->SwapTextures();
		Texture* baseTexture = _textureForBlending->GetSourceTexture();
		Texture* detailTexture = detail->GetRenderable()->GetTexture();

		glm::mat3 transform = detail->GetTransform();
		shader->SetMatrix3("toDetailTexCoord", transform);

		shader->SetInteger("baseTexture", 0);
		baseTexture->SetUnitIndex(0);
		baseTexture->Render();

		shader->SetInteger("detailTexture", 1);
		detailTexture->SetUnitIndex(1);
		detailTexture->Render();

		framebuffer->SetRenderTarget(_textureForBlending->GetTargetTexture());
		framebuffer->Bind();

		forBaseTexture->BindTexture(nullptr);
		forBaseTexture->Render();
	}

	if (dumpFlag)
	{
		std::string fileName = "texture" + std::to_string(_textureForBlending->GetTargetTexture()->GetID()) + ".ppm";
		framebuffer->DumpFBO2PPM(fileName.c_str());
	}
	framebuffer->Unbind();
}

void Rectangle::SetNormalBlendMode(NBM mode)
{
	_normalBlendMode = mode;
	LogCurrentMode();
	BlendNormalMap();
}

void Rectangle::ToggleNormalBlendMode()
{
	_normalBlendMode = (NBM)((_normalBlendMode + 1) % MODE_NUM);
	LogCurrentMode();
	BlendNormalMap();
}

void Rectangle::LogCurrentMode()
{
	std::cout << "Current blend mode: ";
	switch (_normalBlendMode)
	{
	case LINEAR:
		std::cout << "LINEAR";
		break;
	case OVERLAY:
		std::cout << "OVERLAY";
		break;
	case PARTIAL_DERIVATIVE:
		std::cout << "PARTIAL_DERIVATIVE";
		break;
	case WHITEOUT:
		std::cout << "WHITEOUT";
		break;
	case UDN:
		std::cout << "UDN";
		break;
	case REORIENTED:
		std::cout << "REORIENTED";
		break;
	case UNITY:
		std::cout << "UNITY";
		break;
	}
	std::cout << std::endl;
}