#include "Object.h"


Object::Object(GLfloat)
{
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);
}