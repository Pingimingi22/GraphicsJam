#include "ShaderManager.h"
#include "glad/gl.h"
#include "Renderer.h"

unsigned int ShaderManager::GetProgram(
	std::string programName) {

	return _shaderPrograms[programName];
}

void ShaderManager::CreateShaderProgram(
	std::string programName,
	std::string vertexShaderPath,
	std::string fragmentShaderPath) {

	unsigned int vertexShader = CreateShader(ShaderType::Vertex, vertexShaderPath);
	unsigned int fragmentShader = CreateShader(ShaderType::Fragment, fragmentShaderPath);

	// Creating the shader program.
	unsigned int newShaderProgram = glCreateProgram();
	glAttachShader(newShaderProgram, vertexShader);
	glAttachShader(newShaderProgram, fragmentShader);
	glLinkProgram(newShaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(newShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(newShaderProgram, 512, NULL, infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Add shader program to our cache.
	_shaderPrograms[programName] = newShaderProgram;
}

unsigned int ShaderManager::CreateShader(
	ShaderType type,
	std::string filepath) {
	
	std::string source = ReadShader(filepath);
	unsigned int shader;
	std::string shaderTypeString;
	
	if (type == ShaderType::Fragment) {
		shader = glCreateShader(GL_FRAGMENT_SHADER);
		shaderTypeString = "FRAGMENT";
	}
	else {
		shader = glCreateShader(GL_VERTEX_SHADER);
		shaderTypeString = "VERTEX";
	}
	
	const char* sourceC = source.c_str();

	glShaderSource(shader, 1, &sourceC, NULL);
	glCompileShader(shader);

	{
		int success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << 
				"ERROR::SHADER::COMPILATION_FAILED" << 
				shaderTypeString << 
				"\n" << 
				infoLog << 
				std::endl;
		}
	}

	return shader;
}

std::string ShaderManager::ReadShader(
	std::string path) {

	std::string fullFile;
	std::string line;
	std::ifstream shaderFile(path);

	if (shaderFile.is_open())
	{
		while (std::getline(shaderFile, line)) {
			//std::cout << line << '\n';
			fullFile.append(line + "\n");
		}
		shaderFile.close();
	}
	else {
		std::cout << "Unable to open shader file" << std::endl;
	}

	return fullFile;
}

void ShaderManager::UseShaderProgram(
	std::string programName,
	unsigned int texture0,
	std::optional<unsigned int> texture1,
	std::optional<glm::mat4> modelMat,
	std::optional<bool> isHighlighted) {

	unsigned int shaderProgram = GetProgram(programName);

	glUseProgram(shaderProgram);

	// Apply uniforms.
	if (modelMat.has_value()) {
		glUniformMatrix4fv(
			glGetUniformLocation(
				shaderProgram, 
				"modelMat"), 
			1, 
			GL_FALSE, 
			&modelMat.value()[0][0]);
	}

	Renderer::Camera camera = Renderer::Instance->camera;

	glUniformMatrix4fv(
		glGetUniformLocation(
			shaderProgram, 
			"viewMat"), 
		1, 
		GL_FALSE, 
		&camera.GetViewMatrix()[0][0]);
	glUniformMatrix4fv(
		glGetUniformLocation(
			shaderProgram, 
			"projectionMat"), 
		1, 
		GL_FALSE, 
		&camera.projectionMatrix[0][0]);

	if (isHighlighted.has_value()) {
		glUniform1i(
			glGetUniformLocation(
				shaderProgram, 
				"isHighlighted"), 
			isHighlighted.value());
	}

	glActiveTexture(GL_TEXTURE0);
	if (texture0 != -1) {
		glBindTexture(GL_TEXTURE_2D, texture0);
		glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
	}

	if (texture1.has_value()) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1.value());
		glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);
	}

	// Go back to texture 0.
	//glActiveTexture(GL_TEXTURE0);
}