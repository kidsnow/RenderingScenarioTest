#pragma once

#include "Application.h"

#include <vector>

class Texture;
class Rectangle;

class MultipleTexturesApp : public Application
{
public:
	MultipleTexturesApp();
	virtual ~MultipleTexturesApp();

protected:
	virtual void Update();

private:
	int _texCountX;
	int _texCountY;
	int _texCount;
	glm::vec2 _rectangleSize;
	Rectangle* _rectangle;
	std::vector<Texture*> _textures;
};