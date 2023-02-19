#include "FramebufferApp.h"

#include "glm/vec2.hpp"
#include "Rectangle.h"
#include "ShaderManager.h"
#include "Framebuffer.h"

FramebufferApp::FramebufferApp() :
	_rectangle(nullptr)
{
	_rectangle = new Rectangle(glm::vec2(1, 1));

	_framebuffer = new Framebuffer(320, 240, 4);

	_framebuffer->AddRenderbuffer(Framebuffer::Attachment::Color(0));
	_framebuffer->AddRenderbuffer(Framebuffer::Attachment::Color(1));
	_framebuffer->AddRenderbuffer(Framebuffer::Attachment::Color(2));
	_framebuffer->AddRenderbuffer(Framebuffer::Attachment::DepthStencil);
	
	if (_framebuffer->IsComplete())
	{
		printf("Frame buffer is ready to use.\n");
	}
	else
	{
		printf("Not ready.\n");
	}
}

FramebufferApp::~FramebufferApp()
{

}

void FramebufferApp::processKeyInput()
{
	Application::processKeyInput();
}

void FramebufferApp::processMouseInput()
{
	Application::processMouseInput();
}

void FramebufferApp::Update()
{
	_framebuffer->Bind();
	glClearColor(0.3, 0.3, 0.3, 1.0);
	CHECK_GL_ERROR
	glClear(GL_COLOR_BUFFER_BIT);
	Shader* shader = ShaderManager::GetShader("framebuffer");

	CHECK_GL_ERROR
	shader->Use();

	CHECK_GL_ERROR
	_rectangle->Render();

	CHECK_GL_ERROR
	_framebuffer->DumpBuffer("C:\\Zpac\\DumpedBuffer");
}