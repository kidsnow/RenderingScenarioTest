#include "NormalBlendingApp.h"

#include "ShaderManager.h"
#include "Camera.h"
#include "Renderable.h"
#include "Texture.h"


NormalBlendingApp::NormalBlendingApp() :
	_camera(nullptr),
	_baseRectangle(nullptr),
	_detailRectangle(nullptr),
	_baseTexture(nullptr),
	_detailTexture(nullptr),
	_printItOut(false)
{
	_camera = new Camera();
	glm::quat rot = _camera->GetQuaternion();
	_camera->SetPosition(glm::vec4(0.0, 0.0, 500.0, 1.0));
	_camera->SetFrustum(glm::vec2(320, 180), 100, 1000);

	_baseRectangle = new Renderable(glm::vec2(320, 320));
	_baseRectangle->SetPosition(glm::vec4(0.0, 0.0, 0.0, 1.0));
	_baseTexture = new Texture("Base.jpg");
	_baseRectangle->BindTexture(_baseTexture);

	_detailRectangle = new Renderable(glm::vec2(320, 320));
	_detailRectangle->SetPosition(glm::vec4(0.0, 0.0, 0.0, 1.0));
	_detailTexture = new Texture("DetailNormalMap.png");
	_detailRectangle->BindTexture(_detailTexture);

	_baseRectangle->AddDetail(_detailRectangle);
}

NormalBlendingApp::~NormalBlendingApp()
{
}

float curColor = 0.0f;

void NormalBlendingApp::processKeyInput()
{
	if (IsPressed(KEY_W))
	{
		_camera->Move(glm::vec3(0.0, 1.0, 0.0));
	}
	if (IsPressed(KEY_A))
	{
		_camera->Move(glm::vec3(-1.0, 0.0, 0.0));
	}
	if (IsPressed(KEY_S))
	{
		_camera->Move(glm::vec3(0.0, -1.0, 0.0));
	}
	if (IsPressed(KEY_D))
	{
		_camera->Move(glm::vec3(1.0, 0.0, 0.0));
	}
	if (IsPressed(KEY_Q))
	{
		_camera->Move(glm::vec3(0.0, 0.0, -1.0));
	}
	if (IsPressed(KEY_E))
	{
		_camera->Move(glm::vec3(0.0, 0.0, 1.0));
	}
	if (IsPressed(KEY_P))
	{
		_printItOut = true;
	}
}

void NormalBlendingApp::Update()
{
	processKeyInput();

	if (_printItOut)
	{
		_baseRectangle->BlendNormalMap(true);
		_printItOut = false;
	}

	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, _window->GetWidth(), _window->GetHeight());
	glClearColor(0.4f, 0.4f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Shader* shader = ShaderManager::Instance()->GetShader("simple");
	shader->Use();

	glm::mat4 view = _camera->GetInvTransform();
	glm::mat4 proj = _camera->GetProjectionTransform();

	shader->SetMatrix4("MVP", proj*view*_baseRectangle->GetTransform());

	_baseRectangle->Render();
}