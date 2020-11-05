#include "application.h"
#include "NormalBlendingApp.h"


int main(int argc, char** argv)
{
	Application* app = new NormalBlendingApp();

	app->Run();

	delete app;

	return 0;
}