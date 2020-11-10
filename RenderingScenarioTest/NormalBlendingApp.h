#pragma once

#include "Application.h"

class Camera;
class Renderable;
class Texture;


class NormalBlendingApp : public Application
{
public:
	NormalBlendingApp();
	virtual ~NormalBlendingApp();

public:
	virtual void Update();

private:
	void processKeyInput();

private:
	Camera* _camera;
	Renderable* _baseRectangle;
	Renderable* _detailRectangle;
	Texture* _baseTexture;
	Texture* _detailTexture;
	bool _printItOut;
};