#pragma once

#include "Object.h"
#include "Texture.h"
#include "GL/glew.h"
#include <vector>

class Texture;

class TextureForBlending
{
public:
	TextureForBlending(int width, int height);
	~TextureForBlending();

public:
	void SwapTextures();
	Texture* GetTargetTexture() { return _texture[_target]; }
	Texture* GetSourceTexture() { return _texture[_source]; }

private:
	int _width, _height;
	int _target, _source;
	Texture* _texture[2];
};

typedef enum NormalBlendMode
{
	DIFFERENTIAL,
	UDN
} NBM;

class Renderable : public Object
{
public:
	Renderable(glm::vec2 size);
	~Renderable();

private:
	void initializeVAO();

public:
	void Render();
	void BindTexture(Texture* texture);
	void AddDetail(Renderable* detail);
	void SetNormalBlendMode(NBM mode) { _normalBlendMode = mode; }
	void BlendNormalMap(bool dumpFlag = false);
	Texture* GetTexture() { return _texture; }

private:
	glm::vec2 _size;
	GLuint _vao;
	GLfloat* _vertices;
	Texture* _texture;
	TextureForBlending* _textureForBlending;
	std::vector<Renderable*> _detailList;
	NBM _normalBlendMode;
};