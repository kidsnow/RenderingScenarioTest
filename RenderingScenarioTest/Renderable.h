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
	// _vertices�� geometry������ ����ִ� ��Ȳ������ ȣ���ؾ� �Ѵ�.
	void initializeVAO();

	GLfloat* _vertices;
	GLuint _vao;
	Texture* _texture;
};