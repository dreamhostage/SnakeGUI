#pragma once

enum Type
{
	snakeHead, snakeBody
};

enum PlayerType
{
	P1, P2
};

struct History
{
	History(float& x, float& y)
	{
		this->x = x;
		this->y = y;
	}
	float x;
	float y;
	float rotation;
};
