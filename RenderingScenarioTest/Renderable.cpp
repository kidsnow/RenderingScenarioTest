#include "Renderable.h"
#include "Texture.h"


Renderable::Renderable(glm::vec2 size) :
	_size(size),
	_vao(0),
	_vertices(nullptr),
	_texture(nullptr)
{
	initializeVAO();
}

Renderable::~Renderable()
{
	glDeleteVertexArrays(1, &_vao);
}

void Renderable::initializeVAO()
{
	_vertices = new GLfloat[30]
	{
		-_size.x / 2.0f, -_size.y / 2.0f, 0.0f, 0.0f, 0.0f,	// lower left
		 _size.x / 2.0f, -_size.y / 2.0f, 0.0f, 1.0f, 0.0f,	// lower right
		-_size.x / 2.0f,  _size.y / 2.0f, 0.0f, 0.0f, 1.0f,	// upper left
		-_size.x / 2.0f,  _size.y / 2.0f, 0.0f, 0.0f, 1.0f,	// upper left
		 _size.x / 2.0f, -_size.y / 2.0f, 0.0f, 1.0f, 0.0f,	// lower right
		 _size.x / 2.0f,  _size.y / 2.0f, 0.0f, 1.0f, 1.0f,	// upper right
	};

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 5, _vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderable::Render()
{
	if (_texture != nullptr)
	{
		_texture->Render();
	}
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glFinish();
}

void Renderable::BindTexture(Texture* texture)
{
	_texture = texture;
}

void Renderable::AddDetail(Renderable* detail)
{
	_detailList.push_back(detail);
}