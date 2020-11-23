#include "application.h"
#include "MultipleTexturesApp.h"


int main(int argc, char** argv)
{
	Application* app = new MultipleTexturesApp();

	app->Run();

	delete app;

	return 0;
}