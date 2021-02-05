#include "MultipleTexturesApp.h"

#include "ShaderManager.h"
#include "Rectangle.h"
#include "Texture.h"
#include <iostream>

MultipleTexturesApp::MultipleTexturesApp() :
	_texCountX(16),
	_texCountY(16),
	_texCount(0),
	_rectangleSize(300, 300),
	_rectangle(nullptr)
{
	_rectangle = new Rectangle(_rectangleSize);

	int maxTex;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTex);
	std::cout << "Max texture units count supported: " << maxTex << std::endl;
	int maxUniforms;
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxUniforms);
	std::cout << "Max uniform units count supported: " << maxUniforms << std::endl;
	_texCount = _texCountX * _texCountY;
	for (int i = 0; i < _texCount; i++)
	{
		std::string filePath;
		filePath = "resources/MultipleTextures/" + std::to_string(i) + ".ppm";
		Texture* texture = new Texture(filePath.c_str());
		texture->SetUnitIndex(i);
		_textures.push_back(texture);
	}
}

MultipleTexturesApp::~MultipleTexturesApp()
{

}

void MultipleTexturesApp::Update()
{
	glClearColor(0.3, 0.3, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	Shader* shader = ShaderManager::GetShader("multiple_textures");

	shader->Use();
	shader->SetMatrix4("MVP", GetViewProjectionMatrix());
	shader->SetInteger("texCount", _texCount);
	shader->SetInteger("texCountX", _texCountX);
	shader->SetInteger("texCountY", _texCountY);

	for (int i = 0; i < _texCountY; i++)
	{
		for (int j = 0; j < _texCountX; j++)
		{
			int curTexCount = i*_texCountX + j;
			std::string texName;
			texName = "texture" + std::to_string(curTexCount / 16) + "[" + std::to_string(_textures[curTexCount]->GetUnitIndex() - (curTexCount / 16)*16) + "]";
			shader->SetInteger(texName.c_str(), _textures[curTexCount]->GetUnitIndex());
			_textures[curTexCount]->Render();
		}
	}
	_rectangle->Render();
}