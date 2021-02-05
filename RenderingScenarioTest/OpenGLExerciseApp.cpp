#include "OpenGLExerciseApp.h"

OpenGLExerciseApp::OpenGLExerciseApp()
{

}

OpenGLExerciseApp::~OpenGLExerciseApp()
{

}

void OpenGLExerciseApp::Update()
{
	glClearColor(0.3, 0.3, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}