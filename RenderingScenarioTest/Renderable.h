#pragma once

#include "Object.h"
#include "GL/glew.h"
#include <vector>

class Texture;

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
	void blendNormalMap();

public:
	void Render();
	void BindTexture(Texture* texture);
	void AddDetail(Renderable* detail);
	void SetNormalBlendMode(NBM mode) { _normalBlendMode = mode; }

private:
	glm::vec2 _size;
	GLuint _vao;
	GLfloat* _vertices;
	Texture* _texture;
	Texture* _blendedTexture;
	std::vector<Renderable*> _detailList;
	NBM _normalBlendMode;
};