#include "World.h"
#include "glm/vec3.hpp"
#include "Window.h"
#include "PhysicsBody.h"
#include "Application.h"

void World::Init(b2WorldId worldId)
{
	/*for (int i = 0; i < _width; i++) {
		for (int j = 0; j < _height; j++) {
			tiles.emplace_back(
				"grass.png",
				glm::vec3(i, j - _height/2, 0));
		}
	}*/
}

void World::CreateWorldFromImage(std::string filename)
{
	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 4);


	/*std::cout << (int)data[0] << std::endl;
	std::cout << (int)data[1] << std::endl;
	std::cout << (int)data[2] << std::endl;
	std::cout << (int)data[3] << std::endl;
	std::cout << (int)data[4] << std::endl;
	std::cout << (int)data[5] << std::endl;
	std::cout << (int)data[6] << std::endl;
	std::cout << (int)data[7] << std::endl;*/

	struct Colour {
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;

		Colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
			: r(r), g(g), b(b), a(a) {}

		bool Compare(Colour otherColour) {
			return r == otherColour.r &&
				g == otherColour.g &&
				b == otherColour.b &&
				a == otherColour.a;
		}

		void Print() {
			std::cout << "R: " << (int)r << ", G: " << (int)g << ", B: " << (int)b << ", A: " << (int)a << '\n';
		}
	};


	Colour green = Colour(38, 127, 0, 255);
	Colour black = Colour(0, 0, 0, 255);
	Colour brown = Colour(127, 51, 0, 255);
	Colour white = Colour(255, 255, 255, 255);

	Colour* thing = (Colour*)data;
	for (int i = 0; i < (width*height); i++) {
		
		Colour theColour = thing[i];
		theColour.Print();

		int x = i % width;
		int y = floor(i / width);

		if (theColour.Compare(green)) {
			CreateTile(TileType::Grass, x, y);
		}
		else if (theColour.Compare(black)) {
			CreateTile(TileType::Stone, x, y);
		}
		else if (theColour.Compare(brown)) {
			CreateTile(TileType::Dirt, x, y);
		}
		else if (theColour.Compare(white)) {
			// Don't create anything.
		}
	}

	stbi_image_free(data);
}

void World::CreateTile(TileType tile, int x, int y)
{
	float tileScale = 2.0f;

	std::string tileTexturePath = "";

	switch (tile) {
	case TileType::Dirt:
		tileTexturePath = "dirt.png";
		break;
	case TileType::Stone:
		tileTexturePath = "stone.png";
		break;
	case TileType::Grass:
		tileTexturePath = "grass.png";
		break;
	}

	PhysicsBody tilePhysics = PhysicsBody(
		Application::Instance->WorldId,
		b2_staticBody,
		glm::vec2(x * tileScale, y * tileScale),
		1.0f,
		1.0f,
		true);
	Sprite sprite = Sprite(tileTexturePath, glm::vec3(x * tileScale, y * tileScale, 0), glm::vec3(tileScale, tileScale, 1.0f));
	tiles.emplace_back(sprite, tilePhysics);
}

Tile::Tile(Sprite sprite, PhysicsBody physics)
: sprite(sprite), physics(physics) {

}
