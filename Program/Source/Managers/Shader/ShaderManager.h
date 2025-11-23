#pragma once
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include "glm/common.hpp"
#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/mat3x3.hpp"
#include <optional>

static class ShaderProgramNames {
public:
	static const inline std::string Basic = "basic";
	static const inline std::string Flipbook = "flipbook";
	static const inline std::string Line = "line";
	static const inline std::string PointToPoint = "pointToPoint";
	static const inline std::string Shadowcast = "shadowCast";
};

enum class ShaderType {
	Fragment,
	Vertex
};

static class ShaderManager
{
public:
	static unsigned int GetProgram(
		std::string programName);

	static void UseShaderProgram(
		std::string programName,
		unsigned int texture,
		std::optional<glm::mat4> modelMat = std::nullopt,
		std::optional<bool> isHighlighted = std::nullopt);

	static void CreateShaderProgram(
		std::string programName,
		std::string vertexShaderPath,
		std::string fragmentShaderPath);

private:
	static inline std::map<std::string, unsigned int> 
		_shaderPrograms;

	static unsigned int CreateShader(
		ShaderType type,
		std::string filepath);

	static std::string ReadShader(std::string path);
};
