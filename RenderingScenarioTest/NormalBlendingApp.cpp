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

float curColor = 0.0f;

void NormalBlendingApp::Update()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.4f, 0.4f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	_rectangle->Render();
	_texture->Render();
	_shader->Use();
	curColor += 0.1f;
	if (curColor > 1.0f)
		curColor = 0.0f;
	_shader->SetFloat("curColor", curColor);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glFinish();
}