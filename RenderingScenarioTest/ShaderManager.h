#pragma once

#include "Shader.h"
#include <map>
#include <string>


typedef enum ShaderType
{
	SIMPLE,
	NORMAL_BLEND
};

class ShaderManager
{
private:
	ShaderManager();
	~ShaderManager();

private:
	void addShader(std::string name);

public:
	static ShaderManager* Instance();
	static Shader* GetShader(std::string name);

private:
	std::map<std::string, Shader*> _shaders;
};