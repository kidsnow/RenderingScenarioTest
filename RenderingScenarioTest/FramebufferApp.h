#pragma once

#include "Application.h"

class Rectangle;
class Framebuffer;

class FramebufferApp : public Application
{
public:
	FramebufferApp();
	virtual ~FramebufferApp();

public:
	virtual void Update();

private:
	virtual void processKeyInput();
	virtual void processMouseInput();

private:
	Rectangle* _rectangle;
	Framebuffer* _framebuffer;
};