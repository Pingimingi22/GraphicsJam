#include "Sprite.h"

Sprite::Sprite(std::string texturePath, glm::vec3 position)
{
	_position = position;
	rotation = 0.0f;

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

glm::mat4 Sprite::ObjToWorld()
{
	glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), _position) * 
						 glm::scale(glm::mat4(1.0f), _scale) * 
						 glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	return modelMat;
}
