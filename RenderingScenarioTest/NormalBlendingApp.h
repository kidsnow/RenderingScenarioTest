#pragma once

#include "Application.h"


class NormalBlendingApp : public Application
{
public:
	NormalBlendingApp();
	virtual ~NormalBlendingApp();

public:
	virtual void Initialize();
	virtual void Run();
};