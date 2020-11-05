#pragma once

#include "Application.h"

class Object;
class Texture;


class NormalBlendingApp : public Application
{
public:
	NormalBlendingApp();
	virtual ~NormalBlendingApp();

public:
	virtual void Update();

private:
	Object* _rectangle;
	Texture* _texture;
};