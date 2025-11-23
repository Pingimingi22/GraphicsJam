#pragma once
#include <string>

class ShaderProgram
{
public:
	std::string GetShaderName();
	std::string GetShaderFilename();

private:
	std::string _shaderName;
	std::string _shaderFilename;
	unsigned int program;
};

