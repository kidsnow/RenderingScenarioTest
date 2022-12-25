#pragma once

#include "Application.h"


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
	
};