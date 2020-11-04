#pragma once

#include "GL/glew.h"


class Object
{
public:
	Object();
	virtual ~Object();

private:
	GLuint _VAO;
};