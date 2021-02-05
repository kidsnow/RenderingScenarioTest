#pragma once

#include "Application.h"

#include <vector>

class OpenGLExerciseApp : public Application
{
public:
	OpenGLExerciseApp();
	virtual ~OpenGLExerciseApp();

protected:
	virtual void Update() override;

private:
};