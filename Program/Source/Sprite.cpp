#include "Sprite.h"

Sprite::Sprite(std::string texturePath, glm::vec3 position, b2WorldId worldId, b2BodyType type)
{

	_position = position;
	rotation = 0.0f;

	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = type;
	b2Vec2 bodyPosition;
	bodyPosition.x = position.x;
	bodyPosition.y = position.y;
	bodyDef.position = bodyPosition;
	bodyDef.fixedRotation = true;
	b2Polygon dynamicBox = b2MakeRoundedBox(0.5f, 0.5f, 0.25f);

	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 1.0f;
	shapeDef.friction = 0.1f;
	

	_bodyId = b2CreateBody(worldId, &bodyDef);

	b2CreatePolygonShape(_bodyId, &shapeDef, &dynamicBox);


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
	b2Vec2 bodyPosition = b2Body_GetPosition(_bodyId);
	glm::vec4 physicsPosition = glm::vec4(bodyPosition.x, bodyPosition.y, 0.0f, 1.0f);

	_position.x = bodyPosition.x;
	_position.y = bodyPosition.y;
	glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), _position) * 
						 glm::scale(glm::mat4(1.0f), _scale) * 
						 glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	return modelMat;
}
