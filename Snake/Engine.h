#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <cmath>

#include "Snake.h"
#include "Eat.h"

class Engine
{
	int screenX;
	int screenY;
	std::vector<std::unique_ptr<Snake>> snakeP1;
	std::vector<std::unique_ptr<Snake>> snakeP2;
	std::vector<std::unique_ptr<Eat>> eatBase;
	std::unique_ptr<sf::RenderWindow> window;
	sf::Texture themeTexture;
	std::unique_ptr<sf::Texture> P1SettingsTexture;
	std::unique_ptr<sf::Texture> P2SettingsTexture;
	sf::Sprite themeSprite;
	std::unique_ptr<sf::Sprite> P1SettingsSprite;
	std::unique_ptr<sf::Sprite> P2SettingsSprite;
	float angle;
	sf::Clock P1snakeMovementTime;
	sf::Clock P2snakeMovementTime;
	sf::Clock updatingTime;
	sf::Text text;
	sf::Font font;
	float newPosX;
	float newPosY;
	float newRotation;
	float P1SnakeSpeed;
	float P2SnakeSpeed;
	float newSpeed;
	bool isP1Moving;
	bool isP2Moving;
	bool updating;
	bool gameStarted;

	void draw();
	void SnakeEngine(std::vector<std::unique_ptr<Snake>>& snake, PlayerType player, sf::Clock& snakeMovementTime);
	void generateEat();
	void checkEatClosing(std::vector<std::unique_ptr<Snake>>& snake, PlayerType player);
	void eatEngine();
	void addSnakePart(std::vector<std::unique_ptr<Snake>>& snake, PlayerType player);
	void checkCollision(std::vector<std::unique_ptr<Snake>>& snakeP1, std::vector<std::unique_ptr<Snake>>& snakeP2, PlayerType player);
	void recreateSnakes();

public:

	Engine();
	void run();
};
