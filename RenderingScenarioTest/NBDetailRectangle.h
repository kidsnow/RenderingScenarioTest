#pragma once

#include "glm/mat3x3.hpp"

class Rectangle;

typedef enum NBDetailType
{
	DETAIL_TOP,
	DETAIL_BOTTOM,
	DETAIL_LEFT,
	DETAIL_RIGHT,
	DETAIL_CENTER
};

class NBDetailRectangle
{
public:
	NBDetailRectangle(NBDetailType type, Rectangle* rectangle) :
		_type(type), _rectangle(rectangle)
	{}
	~NBDetailRectangle() {}

public:
	NBDetailType GetType() { return _type; }
	Rectangle* GetRenderable() { return _rectangle; }
	glm::mat3 GetTransform();

private:
	NBDetailType _type;
	Rectangle* _rectangle;
};