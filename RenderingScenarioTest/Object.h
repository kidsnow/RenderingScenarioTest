#pragma once

#include "GL/glew.h"


class Object
{
public:
	Object();
	virtual ~Object();

public:
	void Render();

private:
	GLuint _vao;
};