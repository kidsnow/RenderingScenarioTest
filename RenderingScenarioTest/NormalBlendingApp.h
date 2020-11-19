#pragma once

#include "Application.h"
#include "glm/vec2.hpp";

class Camera;
class NBRectangle;
class Texture;


class NormalBlendingApp : public Application
{
public:
	NormalBlendingApp();
	virtual ~NormalBlendingApp();

public:
	virtual void Update();

private:
	virtual void processKeyInput();
	virtual void processMouseInput();

private:
	Camera* _camera;
	glm::vec2 _frustumSize;
	float _near;
	float _far;
	NBRectangle* _baseRectangle;
	NBRectangle* _detailRectangle;
	NBRectangle* _detailRectangle2;
	Texture* _baseTexture;
	Texture* _detailTexture;
	Texture* _detailTexture2;
	bool _printItOut;
};