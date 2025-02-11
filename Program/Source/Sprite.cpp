#include "Sprite.h"

Sprite::Sprite(std::string texturePath, glm::vec3 position, glm::vec2 scale /*= glm::vec2(1.0f)*/)
{
	_position = position;
	rotation = 0.0f;
	SetScale(scale);

	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 4);

	glGenTextures(1, &_texId);

	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, _texId);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture." << std::endl;
	}
	stbi_image_free(data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


}

GLuint Sprite::GetTextureId()
{
	return _texId;
}

glm::vec2 Sprite::GetPosition()
{
	return glm::vec2(_position.x, _position.y);
}

void Sprite::SetPosition(glm::vec2 newPosition)
{
	_position.x = newPosition.x;
	_position.y = newPosition.y;
}

void Sprite::SetScale(float uniformScale)
{
	_scale.x = uniformScale;
	_scale.y = uniformScale;
	_scale.z = 1.0f;
}

void Sprite::SetScale(glm::vec2 nonUniformScale)
{
	_scale.x = nonUniformScale.x;
	_scale.y = nonUniformScale.y;
	_scale.z = 1.0f;
}

glm::mat4 Sprite::ObjToWorld()
{
	glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), _position) * 
						 glm::scale(glm::mat4(1.0f), _scale) * 
						 glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	return modelMat;
}

glm::mat4 Sprite::WorldToObj()
{
	return glm::inverse(ObjToWorld());
}
