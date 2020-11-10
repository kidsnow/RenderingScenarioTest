#pragma once

#include "Object.h"
#include "GL/glew.h"
#include <vector>

class Texture;


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

private:
	glm::vec2 _size;
	GLuint _vao;
	GLfloat* _vertices;
	Texture* _texture;
	std::vector<Renderable*> _detailList;
};