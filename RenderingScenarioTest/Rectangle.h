#pragma once

#include "Renderable.h"
#include "NBDetailRectangle.h"
#include "glm/vec2.hpp"
#include <vector>

class TextureForBlending;
class Renderable;

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

class Rectangle : public Renderable
{
public:
	Rectangle(glm::vec2 size);
	virtual ~Rectangle();

	virtual void Render();

private:
	void createVertexArray();

	glm::vec2 _size;
};