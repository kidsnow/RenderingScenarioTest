#include "NormalBlendingApp.h"

#include "Object.h"
#include "Texture.h"


NormalBlendingApp::NormalBlendingApp() :
	_rectangle(nullptr),
	_texture(nullptr)
{
	_shader->CompileRenderingShader("simple.vert", "simple.frag");
	_rectangle = new Object();
	_texture = new Texture("PolygonPlanet.png");
}

NormalBlendingApp::~NormalBlendingApp()
{
}

void NormalBlendingApp::Update()
{
	glClearColor(0.4f, 0.4f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	_shader->Use();
	_texture->Render();
	_rectangle->Render();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glFinish();
}