#pragma once

#include "Application.h"

class Rectangle;
class Framebuffer;

/*
 * ToDo:
 * Multiple Color Attachment 구현하고 테스트할 수 있는 환경 구현하기
 * Depth와 Stencil Test를 할 수 있는 환경 만들어서 Renderbuffer, Texture 둘 다 구현하기
 */

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