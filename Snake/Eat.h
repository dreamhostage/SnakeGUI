#pragma once
#include <SFML/Graphics.hpp>

class Eat
{
public:
	std::unique_ptr<sf::Texture> texture;
	std::unique_ptr<sf::Sprite> sprite;
	bool destroyed;

	Eat(int screenX, int screenY, int x = 0, int y = 0, int rotation = 0);
};
