#pragma once

#include "Object.h"
#include "Texture.h"
#include "GL/glew.h"
#include <vector>

class Texture;
class Renderable;

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

typedef enum DetailType
{
	DETAIL_TOP,
	DETAIL_BOTTOM,
	DETAIL_LEFT,
	DETAIL_RIGHT,
	DETAIL_CENTER
};

class DetailRenderable
{
public:
	DetailRenderable(DetailType type, Renderable* renderable) :
		_type(type), _renderable(renderable)
	{}
	~DetailRenderable() {}

public:
	DetailType GetType() { return _type; }
	Renderable* GetRenderable() { return _renderable; }
	glm::mat3 GetTransform();

private:
	DetailType _type;
	Renderable* _renderable;
};

typedef enum NormalBlendMode
{
	LINEAR,
	OVERLAY,
	PARTIAL_DERIVATIVE,
	WHITEOUT,
	UDN,
	REORIENTED,
	UNITY,
	MODE_NUM
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
	void AddDetail(DetailType type, Renderable* detail);
	void LogCurrentMode();
	void SetNormalBlendMode(NBM mode);
	void ToggleNormalBlendMode();
	void BlendNormalMap(bool dumpFlag = false);
	Texture* GetTexture() { return _texture; }

private:
	glm::vec2 _size;
	GLuint _vao;
	GLfloat* _vertices;
	Texture* _texture;
	TextureForBlending* _textureForBlending;
	std::vector<DetailRenderable*> _detailList;
	NBM _normalBlendMode;
};