#include "Engine.h"
#include <iostream>

Engine::Engine()
{
    srand(time(NULL));
    screenX = sf::VideoMode::getDesktopMode().width;
    screenY = sf::VideoMode::getDesktopMode().height;
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(screenX, screenY), "SFML window");
    window->setFramerateLimit(30);

    font.loadFromFile("images/18949.ttf");
    text.setFont(font);
    text.setCharacterSize(screenX / 64);
    text.setFillColor(sf::Color::White);

    recreateSnakes();

    themeTexture.loadFromFile("images/Theme.png");
    themeSprite.setTexture(themeTexture);
    themeSprite.setScale(screenX / themeSprite.getLocalBounds().width, screenY / themeSprite.getLocalBounds().height);

    isP1Moving = false;
    isP2Moving = false;
    updating = false;
    gameStarted = false;

    P1SettingsTexture = std::make_unique<sf::Texture>();
    P1SettingsSprite = std::make_unique<sf::Sprite>();
    P1SettingsTexture->loadFromFile("images/P1Settings.png");
    P1SettingsSprite->setTexture(*P1SettingsTexture);
    auto sizeX = P1SettingsTexture->getSize().x;
    auto sizeY = P1SettingsTexture->getSize().y;
    P1SettingsSprite->setOrigin(sizeX / 2, sizeY / 2);
    P1SettingsSprite->setScale((screenX / 100 * 0.03), (screenX / 100 * 0.03));

    P2SettingsTexture = std::make_unique<sf::Texture>();
    P2SettingsSprite = std::make_unique<sf::Sprite>();
    P2SettingsTexture->loadFromFile("images/P2Settings.png");
    P2SettingsSprite->setTexture(*P2SettingsTexture);
    sizeX = P2SettingsTexture->getSize().x;
    sizeY = P2SettingsTexture->getSize().y;
    P2SettingsSprite->setOrigin(sizeX / 2, sizeY / 2);
    P2SettingsSprite->setScale((screenX / 100 * 0.03), (screenX / 100 * 0.03));

    P1SettingsSprite->setPosition(snakeP1.begin()->get()->sprite.getPosition().x, snakeP1.begin()->get()->sprite.getPosition().y - P1SettingsTexture->getSize().y / 2);
    P2SettingsSprite->setPosition(snakeP2.begin()->get()->sprite.getPosition().x, snakeP2.begin()->get()->sprite.getPosition().y - P2SettingsTexture->getSize().y / 2);
}

void Engine::recreateSnakes()
{
    gameStarted = false;
    auto it = snakeP1.begin();
    while (it != snakeP1.end())
    {
        it = snakeP1.erase(it);
    }

    it = snakeP2.begin();
    while (it != snakeP2.end())
    {
        it = snakeP2.erase(it);
    }

    snakeP1.push_back(std::make_unique<Snake>(screenX, screenY, "images/snakeHead.png", Type::snakeHead, screenX / 4, screenY / 2));
    isP1Moving = false;

    for (int i = 0; i < 3; ++i)
    {
        int posUnder = snakeP1.back()->sprite.getPosition().y + (snakeP1.back()->texture.getSize().y / 8);
        snakeP1.push_back(std::make_unique<Snake>(screenX, screenY, "images/snakeBody.png", Type::snakeBody, screenX / 4, posUnder));
    }

    snakeP2.push_back(std::make_unique<Snake>(screenX, screenY, "images/snakeHead2.png", Type::snakeHead, screenX * 0.8, screenY / 2));
    isP2Moving = false;

    for (int i = 0; i < 3; ++i)
    {
        int posUnder = snakeP2.back()->sprite.getPosition().y + (snakeP2.back()->texture.getSize().y / 8);
        snakeP2.push_back(std::make_unique<Snake>(screenX, screenY, "images/snakeBody2.png", Type::snakeBody, screenX * 0.8, posUnder));
    }
}

void Engine::checkCollision(std::vector<std::unique_ptr<Snake>>& snakeP1, std::vector<std::unique_ptr<Snake>>& snakeP2, PlayerType player)
{
    auto it = snakeP2.begin();
    sf::Vector2f pos = snakeP1.begin()->get()->sprite.getPosition();

    while (it != snakeP2.end())
    {
        double distanse = sqrt(
            (pos.x - it->get()->sprite.getPosition().x) * (pos.x - it->get()->sprite.getPosition().x)
            + (pos.y - it->get()->sprite.getPosition().y) * (pos.y - it->get()->sprite.getPosition().y));

        if (distanse < 30)
        {
            auto it2 = snakeP1.begin();
            while (it2 != snakeP1.end())
            {
                eatBase.push_back(std::make_unique<Eat>(screenX, screenY, it2->get()->sprite.getPosition().x, it2->get()->sprite.getPosition().y, it2->get()->sprite.getRotation()));
                it2 = snakeP1.erase(it2);
            }

            if (!updating)
            {
                updating = true;
                updatingTime.restart();

                text.setPosition(screenX / 2 - (text.getCharacterSize() * text.getString().getSize()) / 2, 0);

                if (player == PlayerType::P1)
                {
                    text.setString("Player 2 WIN!");
                }
                else
                {
                    text.setString("Player 1 WIN!");
                }
            }

            return;
        }
        else
        {
            ++it;
        }
    }
}

void Engine::addSnakePart(std::vector<std::unique_ptr<Snake>>& snake, PlayerType player)
{
    int posUnder = snake.back()->sprite.getPosition().y + (snake.back()->texture.getSize().y / 4);
    if (player == PlayerType::P1)
    {
        snake.push_back(std::make_unique<Snake>(screenX, screenY, "images/snakeBody.png", Type::snakeBody, -100, -100));
    }
    else
    {
        snake.push_back(std::make_unique<Snake>(screenX, screenY, "images/snakeBody2.png", Type::snakeBody, -100, -100));
    }
}

void Engine::eatEngine()
{
    generateEat();
    auto it = eatBase.begin();
    while (it != eatBase.end())
    {
        if (it->get()->destroyed)
        {
            it->get()->sprite.get()->setScale(it->get()->sprite.get()->getScale().x - 0.001, it->get()->sprite.get()->getScale().y - 0.001);
            if (fabs(it->get()->sprite.get()->getScale().x - 0.1f) < 0.01f)
            {
                it = eatBase.erase(it);
            }
            else
            {
                ++it;
            }
        }
        else
        {
            if (it->get()->sprite.get()->getScale().x < (screenX / 100 * 0.015))
            {
                it->get()->sprite.get()->setScale(it->get()->sprite.get()->getScale().x + 0.0005, it->get()->sprite.get()->getScale().y + 0.0005);
            }
            ++it;
        }
    }
}

void Engine::generateEat()
{
    if (eatBase.size() < 4)
    {
        eatBase.push_back(std::make_unique<Eat>(screenX, screenY));
    }
}

void Engine::SnakeEngine(std::vector<std::unique_ptr<Snake>>& snake, PlayerType player, sf::Clock& snakeMovementTime)
{
    checkEatClosing(snake, player);
    if (snakeMovementTime.getElapsedTime().asMicroseconds() > 3000)
    {
        if (player == PlayerType::P1)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                snake.begin()->get()->sprite.rotate(-0.7);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                snake.begin()->get()->sprite.rotate(0.7);
        }
        else
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                snake.begin()->get()->sprite.rotate(-0.7);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                snake.begin()->get()->sprite.rotate(0.7);
        }

        // Snake moving engine
        auto it = snake.begin();
        while (it != snake.end())
        {
            if (it->get()->type == Type::snakeHead)
            {
                angle = it->get()->sprite.getRotation();
                angle -= (angle - 90) * 2;

                if (player == PlayerType::P1)
                {
                    it->get()->sprite.setPosition(it->get()->sprite.getPosition().x + P1SnakeSpeed * sin((angle)*M_PI / 180), it->get()->sprite.getPosition().y + P1SnakeSpeed * cos((angle)*M_PI / 180));
                }
                else
                {
                    it->get()->sprite.setPosition(it->get()->sprite.getPosition().x + P2SnakeSpeed * sin((angle)*M_PI / 180), it->get()->sprite.getPosition().y + P2SnakeSpeed * cos((angle)*M_PI / 180));
                }
            }
            else
            {
                it->get()->sprite.setPosition((it - 1)->get()->history[(it - 1)->get()->history.size() - 1].x, (it - 1)->get()->history[(it - 1)->get()->history.size() - 1].y);
                it->get()->sprite.setRotation((it - 1)->get()->history[(it - 1)->get()->history.size() - 1].rotation);

                if (player == PlayerType::P1)
                {
                    it->get()->sprite.setPosition(it->get()->sprite.getPosition().x + P1SnakeSpeed * sin((it->get()->sprite.getRotation())*M_PI / 180), it->get()->sprite.getPosition().y + P1SnakeSpeed * cos((it->get()->sprite.getRotation())*M_PI / 180));
                }
                else
                {
                    it->get()->sprite.setPosition(it->get()->sprite.getPosition().x + P2SnakeSpeed * sin((it->get()->sprite.getRotation())*M_PI / 180), it->get()->sprite.getPosition().y + P2SnakeSpeed * cos((it->get()->sprite.getRotation())*M_PI / 180));
                }
            }
            newPosX = it->get()->sprite.getPosition().x;
            newPosY = it->get()->sprite.getPosition().y;
            newRotation = it->get()->sprite.getRotation();

            for (History& his : it->get()->history)
            {
                std::swap(newPosX, his.x);
                std::swap(newPosY, his.y);
                std::swap(newRotation, his.rotation);
            }

            ++it;
        }

        // Check snake body eating
        it = snake.begin();
        while (it != snake.end())
        {
            double distanse = sqrt(
                (snake.begin()->get()->sprite.getPosition().x - it->get()->sprite.getPosition().x) * (snake.begin()->get()->sprite.getPosition().x - it->get()->sprite.getPosition().x)
                + (snake.begin()->get()->sprite.getPosition().y - it->get()->sprite.getPosition().y) * (snake.begin()->get()->sprite.getPosition().y - it->get()->sprite.getPosition().y));
            if (it != snake.begin() && distanse < it->get()->texture.getSize().y * it->get()->sprite.getScale().y / 3)
            {
                while (it != snake.end())
                {
                    eatBase.push_back(std::make_unique<Eat>(screenX, screenY, it->get()->sprite.getPosition().x, it->get()->sprite.getPosition().y, it->get()->sprite.getRotation()));
                    it = snake.erase(it);
                }
                it = snake.end();
            }
            else
            {
                ++it;
            }
        }
    
        snakeMovementTime.restart();
    }

    // Border checking
    auto it = snake.begin();
    if (snake.begin()->get()->sprite.getPosition().x < 0 ||
        snake.begin()->get()->sprite.getPosition().x > screenX ||
        snake.begin()->get()->sprite.getPosition().y < 0 ||
        snake.begin()->get()->sprite.getPosition().y > screenY)
    {
        while (it != snake.end())
        {
            eatBase.push_back(std::make_unique<Eat>(screenX, screenY, it->get()->sprite.getPosition().x, it->get()->sprite.getPosition().y, it->get()->sprite.getRotation()));
            ++it;
        }
        if (!updating)
        {
            updating = true;
            updatingTime.restart();

            text.setPosition(screenX / 2 - (text.getCharacterSize() * text.getString().getSize()) / 2, 0);

            if (player == PlayerType::P1)
            {
                text.setString("Player 2 WIN!");
            }
            else
            {
                text.setString("Player 1 WIN!");
            }
        }
        snake.clear();
    }
}

void Engine::checkEatClosing(std::vector<std::unique_ptr<Snake>>& snake, PlayerType player)
{
    auto it = eatBase.begin();
    sf::Vector2f pos = snake.begin()->get()->sprite.getPosition();
    while (it != eatBase.end())
    {
        if (!it->get()->destroyed)
        {
            double distanse = sqrt(
                (pos.x - it->get()->sprite.get()->getPosition().x) * (pos.x - it->get()->sprite.get()->getPosition().x)
                + (pos.y - it->get()->sprite.get()->getPosition().y) * (pos.y - it->get()->sprite.get()->getPosition().y));
            if (distanse < it->get()->texture->getSize().x * it->get()->sprite.get()->getScale().x / 2)
            {
                it->get()->destroyed = true;
                addSnakePart(snake, player);
            }
        }
        ++it;
    }
}

void Engine::draw()
{
    window->clear();
    window->draw(themeSprite);

    // Eat
    for (const auto& object : eatBase)
    {
        window->draw(*object->sprite);
    }

    // Snake player 1
    for (const auto& object : snakeP1)
    {
        window->draw(object->sprite);
    }

    // Snake player 1
    for (const auto& object : snakeP2)
    {
        window->draw(object->sprite);
    }

    if (updating)
    {
        window->draw(text);
    }

    if (!gameStarted)
    {
        window->draw(*P1SettingsSprite);
        window->draw(*P2SettingsSprite);
    }

    window->display();
}

void Engine::run()
{
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(screenX, screenY), "SFML window");

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
        }

        draw();

        // Snake moving
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            isP1Moving = true;
            gameStarted = true;
            P1SnakeSpeed = 2;
        }
        else
        {
            P1SnakeSpeed = 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            isP2Moving = true;
            gameStarted = true;
            P2SnakeSpeed = 2;
        }
        else
        {
            P2SnakeSpeed = 1;
        }

        if (isP1Moving && !snakeP1.empty())
        {
            SnakeEngine(snakeP1, PlayerType::P1, P1snakeMovementTime);
        }

        if (isP2Moving && !snakeP2.empty())
        {
            SnakeEngine(snakeP2, PlayerType::P2, P2snakeMovementTime);
        }

        checkCollision(snakeP1, snakeP2, PlayerType::P1);
        checkCollision(snakeP2, snakeP1, PlayerType::P2);

        // Eat behavior
        eatEngine();

        if (updating)
        {
            if (text.getPosition().y < screenY / 2)
                text.move(0, 0.5);
            if (updatingTime.getElapsedTime().asSeconds() > 5)
            {
                recreateSnakes();
                updating = false;
            }
        }
    }
    return;
}