#include "application.h"

#include "FramebufferApp.h"


int main(int argc, char** argv)
{
	Application* app = new FramebufferApp();

	app->Run();

	delete app;

	return 0;
}