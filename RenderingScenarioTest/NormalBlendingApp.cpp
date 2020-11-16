#include "NormalBlendingApp.h"

#include "ShaderManager.h"
#include "Camera.h"
#include "Renderable.h"
#include "Texture.h"


NormalBlendingApp::NormalBlendingApp() :
	_camera(nullptr),
	_frustumSize(960, 540),
	_near(100.0),
	_far(1000.0),
	_baseRectangle(nullptr),
	_detailRectangle(nullptr),
	_baseTexture(nullptr),
	_detailTexture(nullptr),
	_printItOut(false)
{
	_camera = new Camera();
	_camera->SetProjectionMode(CPM::Orthogonal);
	_camera->SetPosition(glm::vec4(0.0, 0.0, 500.0, 1.0));
	_camera->SetFrustum(glm::vec2(960, 540), _near, _far);

	_baseRectangle = new Renderable(glm::vec2(320, 320));
	_baseRectangle->SetPosition(glm::vec4(0.0, 0.0, 0.0, 1.0));
	_baseTexture = new Texture("resources/Top_N.png");
	_baseRectangle->BindTexture(_baseTexture);

	_detailRectangle = new Renderable(glm::vec2(320, 320));
	_detailRectangle->SetPosition(glm::vec4(0.0, 0.0, 0.0, 1.0));
	_detailTexture = new Texture("resources/NormalDetail.png");
	_detailRectangle->BindTexture(_detailTexture);

	_detailRectangle2 = new Renderable(glm::vec2(320, 320));
	_detailRectangle2->SetPosition(glm::vec4(0.0, 0.0, 0.0, 1.0));
	_detailTexture2 = new Texture("resources/brick.png");
	_detailRectangle2->BindTexture(_detailTexture2);

	_baseRectangle->AddDetail(DETAIL_CENTER, _detailRectangle);
	_baseRectangle->AddDetail(DETAIL_CENTER, _detailRectangle2);
}

NormalBlendingApp::~NormalBlendingApp()
{
}

float curColor = 0.0f;

void NormalBlendingApp::processKeyInput()
{
	if (IsPressed(KEY_W))
	{
		_camera->Move(glm::vec3(0.0, 10.0, 0.0));
	}
	if (IsPressed(KEY_A))
	{
		_camera->Move(glm::vec3(-10.0, 0.0, 0.0));
	}
	if (IsPressed(KEY_S))
	{
		_camera->Move(glm::vec3(0.0, -10.0, 0.0));
	}
	if (IsPressed(KEY_D))
	{
		_camera->Move(glm::vec3(10.0, 0.0, 0.0));
	}
	if (IsPressed(KEY_Q))
	{
		_camera->Move(glm::vec3(0.0, 0.0, -10.0));
	}
	if (IsPressed(KEY_E))
	{
		_camera->Move(glm::vec3(0.0, 0.0, 10.0));
	}
	if (IsPressed(KEY_P))
	{
		_printItOut = true;
	}
	if (IsPressed(KEY_M))
	{
		_baseRectangle->ToggleNormalBlendMode();
	}
	for (int i = KEY_1; i < KEY_1 + MODE_NUM; i++)
	{
		if (IsPressed((KEY_LIST)i))
		{
			_baseRectangle->SetNormalBlendMode((NBM)(i - KEY_1));
			return;
		}
	}
}

void NormalBlendingApp::processMouseInput()
{
	if (IsPressed(MOUSE_LEFT))
	{
		_camera->Move(glm::vec3(-GetDeltaOfMouseX(), GetDeltaOfMouseY(), 0.0));
	}
	float mag = GetDeltaOfWheelY() * 0.1;
	_frustumSize *= mag + 1.0;
	_camera->SetFrustum(_frustumSize, _near, _far);
}

void NormalBlendingApp::Update()
{
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