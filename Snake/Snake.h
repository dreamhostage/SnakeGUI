#pragma once
#include <SFML/Graphics.hpp>
#include "Tools.h"
#define M_PI 3.14159265358979323846

class Snake
{
public:

    sf::Texture texture;
    sf::Sprite sprite;
	std::vector<History> history;
	Type type;

	Snake(int screenX, int screenY, const char* texturePath, Type type, float pX, float pY);
};
