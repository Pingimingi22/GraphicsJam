#include "Renderer.h"
#include <iostream>
#include <fstream>


static float _quadVertices[] =
{
	// First triangle
	-0.5f, -0.5f,  0.0f,	0.0f, 0.0f, // btm left
	-0.5f,  0.5f,  0.0f,	0.0f, 1.0f, // top left
	 0.5f,  0.5f,  0.0f,	1.0f, 1.0f, // top right


	// Second triangle
	0.5f,  0.5f,  0.0f,		1.0f, 1.0f, // top right
	0.5f, -0.5f,  0.0f,		1.0f, 0.0f, // bottom right
	-0.5f, -0.5f,  0.0f,	0.0f, 0.0f, // bottom left
};

std::string Renderer::ReadShader(std::string path)
{
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

void Renderer::Init(const Window& window)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_quadVertices), _quadVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glBufferData(GL_ARRAY_BUFFER, sizeof(_quadVertices), _quadVertices, GL_STATIC_DRAW);

	// Vertex shader ---------------------------------

	std::string vertexShaderSource = ReadShader("vertexShader.vsh");
	
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	const char* vertexShaderSourceC = vertexShaderSource.c_str();

	glShaderSource(vertexShader, 1, &vertexShaderSourceC, NULL);
	glCompileShader(vertexShader);

	{
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
	
	// Fragment shader ---------------------------------
	std::string fragmentShaderSource = ReadShader("fragmentShader.fsh");

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* fragmentShaderSourceC = fragmentShaderSource.c_str();

	glShaderSource(fragmentShader, 1, &fragmentShaderSourceC, NULL);
	glCompileShader(fragmentShader);

	{
		int success;
		char infoLog[512];
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	// Creating the shader program.
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// ------------------------

	float halfWidth = 0.5f * 75;
	float halfHeight = (float)window.Height()/window.Width() * 75;
	camera.projectionMatrix = glm::ortho(-halfWidth, halfWidth, -halfHeight/2, halfHeight/2, -100.0f, 100.0f);
	//camera.projectionMatrix = glm::ortho(0.0f, 1.0f * 10, 0.0f, (float)window.Height()/window.Width() * 10, -100.0f, 100.0f);
	camera.position = glm::vec3(0.0f, 0.0f, 10.0f);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);
}

void Renderer::Draw(Sprite sprite, float deltaTime)
{
	// bind vao
	// bind vbo
	// bind texture
	// bind shader
	// set shader uniform, mvp matrix, texture unit
	// draw

	glUseProgram(shaderProgram);

	// Apply uniforms.
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelMat"), 1, GL_FALSE, &sprite.ObjToWorld()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "viewMat"), 1, GL_FALSE, &camera.GetViewMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projectionMat"), 1, GL_FALSE, &camera.projectionMatrix[0][0]);

	glBindTexture(GL_TEXTURE_2D, sprite._texId);
	glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

glm::mat4 Renderer::Camera::GetViewMatrix()
{
	glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), position);
	//glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 0, 1));

	glm::mat4 cameraToWorld = translationMat;
	return glm::inverse(cameraToWorld);
}
