#include "NormalBlendingApp.h"

#include "ShaderManager.h"
#include "NBRectangle.h"
#include "Texture.h"


NormalBlendingApp::NormalBlendingApp() :
	_baseRectangle(nullptr),
	_detailRectangle(nullptr),
	_baseTexture(nullptr),
	_detailTexture(nullptr),
	_printItOut(false)
{

	_baseRectangle = new NBRectangle(glm::vec2(320, 320));
	_baseRectangle->SetPosition(glm::vec4(0.0, 0.0, 0.0, 1.0));
	_baseTexture = new Texture("resources/Top_N.png");
	_baseRectangle->BindTexture(_baseTexture);

	_detailRectangle = new NBRectangle(glm::vec2(320, 320));
	_detailRectangle->SetPosition(glm::vec4(0.0, 0.0, 0.0, 1.0));
	_detailTexture = new Texture("resources/NormalDetail.png");
	_detailRectangle->BindTexture(_detailTexture);

	_detailRectangle2 = new NBRectangle(glm::vec2(320, 320));
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
	Application::processKeyInput();

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
	Application::processMouseInput();
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

	shader->SetMatrix4("MVP", GetViewProjectionMatrix()*_baseRectangle->GetTransform());

	_baseRectangle->Render();
}