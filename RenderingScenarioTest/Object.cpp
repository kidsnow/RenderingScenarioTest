#include "Object.h"


Object::Object() :
	_vao(0)
{
	GLfloat* vertices = new GLfloat[30]
	{
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,	// lower left
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	// lower right
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,	// upper left
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,	// upper left
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	// lower right
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,	// upper right
	};

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 5, vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Object::~Object()
{
	glDeleteVertexArrays(1, &_vao);
}

void Object::Render()
{
	glBindVertexArray(_vao);
}