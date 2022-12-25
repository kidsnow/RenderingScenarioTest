#include "application.h"
#include "MultipleTexturesApp.h"
#include "NormalBlendingApp.h"


int main(int argc, char** argv)
{
	//Application* app = new MultipleTexturesApp();
	Application* app = new NormalBlendingApp();

	app->Run();

	delete app;

	return 0;
}