#include "Snake.h"

Snake::Snake(int screenX, int screenY, const char* texturePath, Type type, float pX, float pY)
{
    texture.loadFromFile(texturePath);
    sprite.setTexture(texture);
    this->type = type;
    auto sizeX = texture.getSize().x;
    auto sizeY = texture.getSize().y;
    sprite.setOrigin(sizeX / 2, sizeY / 2);
    sprite.setScale((screenX / 100 * 0.01), (screenX / 100 * 0.01));
    sprite.setPosition(pX, pY);

    for (int i = 0; i < sizeY / 8; ++i)
    {
        history.push_back(std::move(History(pX, pY)));
        pY += 1;
    }
}
