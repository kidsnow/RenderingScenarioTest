#pragma once

#include "Object.h"
#include "GL/glew.h"

class Texture;

class Renderable : public Object
{
public:
	Renderable();
	virtual ~Renderable();

	virtual void Render();
	void BindTexture(Texture* texture);
	Texture* GetTexture() { return _texture; }

protected:
	// _vertices에 geometry정보가 들어있는 상황에서만 호출해야 한다.
	void initializeVAO();

	GLfloat* _vertices;
	GLuint _vao;
	Texture* _texture;
};