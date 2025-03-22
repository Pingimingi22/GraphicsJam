#pragma once
#include "glm/vec2.hpp"
#include "Sprite.h"
#include <vector>
#include "box2d/box2d.h"
#include <string>
#include "PhysicsBody.h"

enum class TileType {
	Grass, // green (38, 127, 0)
	Stone, // black (0, 0, 0)
	Dirt,  // brown (127, 51, 0)
	Air    // white (1, 1, 1),
};

class Tile {
public:
	Tile(Sprite sprite, PhysicsBody physics);

	Sprite sprite;
	PhysicsBody physics;
};

class World
{
public:
	int _width = 1;
	int _height = 8;

	glm::vec2 worldCentre;

	std::vector<Tile> tiles;
	
	void Init(b2WorldId worldId);

	void CreateWorldFromImage(std::string filename);

private:
	void CreateTile(TileType tile, int x, int y);
	TileType GetTileFromColour(glm::vec3 colour);


};

