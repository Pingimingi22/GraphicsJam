#include "Renderer.h"
#include <iostream>
#include <fstream>
#include "box2d/types.h"

Renderer* Renderer::Instance = nullptr;

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

static float _outlineShape[] =
{
	-0.5f, -0.5f,  0.0f,
	-0.5f,  0.5f,  0.0f,
	0.5f,  0.5f,  0.0f,

	0.5f,  0.5f,  0.0f,
	0.5f, -0.5f,  0.0f,
	-0.5f, -0.5f,  0.0f,
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
	Instance = this;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(_quadVertices), _quadVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// ---------------------------- Testing line drawing gizmo stuff
	glGenVertexArrays(1, &_lineRenderVAO);
	glBindVertexArray(_lineRenderVAO);

	glGenBuffers(1, &_lineRenderVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _lineRenderVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(_quadVertices), _outlineShape, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// -------------------------------

	// ------------------------------- Testing creating circle verts

	glm::vec4 unitVec = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 360; i++) {
		float toRadians = 3.14159265f / 180;
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), (float)i * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec3 newPoint = rotate * unitVec;
		_unitCircleVerts.push_back(newPoint);
	}
	glGenVertexArrays(1, &_circleRenderVAO);
	glBindVertexArray(_circleRenderVAO);

	glGenBuffers(1, &_circleRenderVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _circleRenderVBO);

	glBufferData(GL_ARRAY_BUFFER, _unitCircleVerts.size() * sizeof(float) * 3, _unitCircleVerts.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// -------------------------------

	// Vertex shader ---------------------------------

	
	{
		unsigned int vertexShader = CreateVertexShader("vertexShader.vsh");
		unsigned int fragmentShader = CreateFragmentShader("fragmentShader.fsh");
		_basicShaderProgram = CreateShaderProgram(vertexShader, fragmentShader);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	{
		unsigned int vertexShader = CreateVertexShader("vertexShader.vsh");
		unsigned int fragmentShader = CreateFragmentShader("flipbook.fsh");
		unsigned int flipbookShaderProgram = CreateShaderProgram(vertexShader, fragmentShader);

		_flipbookShaderProgram = flipbookShaderProgram;

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	{
		unsigned int vertexShader = CreateVertexShader("vertexShader.vsh");
		unsigned int fragmentShader = CreateFragmentShader("line.fsh");
		unsigned int lineShaderProgram = CreateShaderProgram(vertexShader, fragmentShader);

		_lineShaderProgram = lineShaderProgram;

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}


	// ------------------------

	float halfWidth = 0.5f * 75;
	float halfHeight = (float)window.Height()/window.Width() * 75;
	camera.projectionMatrix = glm::ortho(-halfWidth, halfWidth, -halfHeight/2, halfHeight/2, -100.0f, 100.0f);
	//camera.projectionMatrix = glm::ortho(0.0f, 1.0f * 10, 0.0f, (float)window.Height()/window.Width() * 10, -100.0f, 100.0f);
	camera.position = glm::vec3(0.0f, 0.0f, 10.0f);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);
}

void Renderer::Draw(Sprite sprite, float deltaTime)
{
	ConfigureShader(
		_basicShaderProgram, 
		sprite.GetTextureId(), 
		sprite.ObjToWorld());

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::DrawGizmo(PhysicsBody body, glm::vec3 colour)
{
	b2Vec2 bodyPos = b2Body_GetPosition(body.GetId());
	b2ShapeType shapeType = b2Shape_GetType(body.GetShapeId());

	if (shapeType == b2_circleShape) {
		b2Circle circle = b2Shape_GetCircle(body.GetShapeId());
		b2Vec2 circleCentre = b2Body_GetPosition(body.GetId());
		float circleRadius = circle.radius;

		glm::vec3 topLeft;
		glm::vec3 topRight;
		glm::vec3 bottomLeft;
		glm::vec3 bottomRight;

		glm::mat4 modelMat =
			glm::translate(
				glm::mat4(1.0f),
				glm::vec3(circleCentre.x, circleCentre.y, 0.0f)) *
			glm::scale(
				glm::mat4(1.0f),
				glm::vec3(circleRadius, circleRadius, 1.0f));

		ConfigureShader(
			_lineShaderProgram,
			-1,
			modelMat);

		glUniform3f(glGetUniformLocation(_lineShaderProgram, "lineColour"),
			colour.x, colour.y, colour.z);

		glBindVertexArray(_circleRenderVAO);

		glLineWidth(2.0f);
		glDrawArrays(GL_LINE_LOOP, 0, _unitCircleVerts.size());
	}
	else if (shapeType == b2_polygonShape) {
		b2AABB aabb = b2Shape_GetAABB(body.GetShapeId());
		b2Vec2 aabbCentre = b2AABB_Center(aabb);
		b2Vec2 aabbsExtents = b2AABB_Extents(aabb);

		glm::vec3 topLeft;
		glm::vec3 topRight;
		glm::vec3 bottomLeft;
		glm::vec3 bottomRight;

		glm::mat4 modelMat =
			glm::translate(
				glm::mat4(1.0f),
				glm::vec3(aabbCentre.x, aabbCentre.y, 0.0f)) *
			glm::scale(
				glm::mat4(1.0f),
				glm::vec3(aabbsExtents.x * 2, aabbsExtents.y * 2, 1.0f));

		ConfigureShader(
			_lineShaderProgram,
			-1,
			modelMat);

		glUniform3f(glGetUniformLocation(_lineShaderProgram, "lineColour"),
			colour.x, colour.y, colour.z);

		glBindVertexArray(_lineRenderVAO);

		glLineWidth(2.0f);

		glDrawArrays(GL_LINE_LOOP, 0, 6);
	}
}

unsigned int Renderer::CreateVertexShader(std::string path)
{
	// Vertex shader ---------------------------------

	std::string vertexShaderSource = ReadShader(path);

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

	return vertexShader;
}

unsigned int Renderer::CreateFragmentShader(std::string path)
{
	// Fragment shader ---------------------------------
	std::string fragmentShaderSource = ReadShader(path);

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

	return fragmentShader;
}

unsigned int Renderer::CreateShaderProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
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

	return newShaderProgram;
}

void Renderer::ConfigureShader(unsigned int shaderProgram, unsigned int texture, glm::mat4 modelMat)
{
	glUseProgram(shaderProgram);

	// Apply uniforms.
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelMat"), 1, GL_FALSE, &modelMat[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "viewMat"), 1, GL_FALSE, &camera.GetViewMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projectionMat"), 1, GL_FALSE, &camera.projectionMatrix[0][0]);

	if (texture != -1) {
		glBindTexture(GL_TEXTURE_2D, texture);
	}
}

void Renderer::Draw(SpriteAnimated sprite, float deltaTime)
{
	ConfigureShader(
		_flipbookShaderProgram, 
		sprite.GetTextureId(), 
		sprite.ObjToWorld());

	glUniform1i(glGetUniformLocation(
		_flipbookShaderProgram, 
		"frames"), 
		8);
	glUniform1i(glGetUniformLocation(
		_flipbookShaderProgram, 
		"currentFrame"), 
		sprite.GetCurrentFrameIndex());

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

glm::mat4 Renderer::Camera::GetViewMatrix()
{
	glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), position);
	//glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 0, 1));

	glm::mat4 cameraToWorld = translationMat;
	return glm::inverse(cameraToWorld);
}

void Renderer::SetCameraPos(float x, float y) {
	camera.position = glm::vec3(x, y, camera.position.z);
}
