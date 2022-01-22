#include "Eat.h"

Eat::Eat(int screenX, int screenY, int x, int y, int rotation)
{
    texture = std::make_unique<sf::Texture>();
    sprite = std::make_unique<sf::Sprite>();
    texture->loadFromFile("images/apple.png");
    sprite->setTexture(*texture);
    auto sizeX = texture->getSize().x;
    auto sizeY = texture->getSize().y;
    sprite->setOrigin(sizeX / 2, sizeY / 2);
    sprite->setScale((screenX / 100 * 0.01), (screenX / 100 * 0.01));
 
    if (!x)
    {
        sprite->setPosition(rand() % screenX, rand() % screenY);
    }
    else
    {
        sprite->setPosition(x, y);
    }
    destroyed = false;
}