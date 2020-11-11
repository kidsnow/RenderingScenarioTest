#include "ShaderManager.h"
#include <iostream>


ShaderManager::ShaderManager()
{
	addShader("simple");
	addShader("normal_blend");
}

ShaderManager::~ShaderManager()
{

}

void ShaderManager::addShader(std::string name)
{
	Shader* shader = new Shader();
	std::string vertShaderName = name + ".vert";
	std::string fragShaderName = name + ".frag";
	shader->CompileRenderingShader(vertShaderName.c_str(), fragShaderName.c_str());
	_shaders[name] = shader;
}

ShaderManager* ShaderManager::Instance()
{
	static ShaderManager* instance = new ShaderManager();
	return instance;
}

Shader* ShaderManager::GetShader(std::string name)
{
	Shader* shader = Instance()->_shaders[name];
	if (shader == nullptr)
	{
		std::cerr << "No such shader." << std::endl;
		exit(0);
	}
}