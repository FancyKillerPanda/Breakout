#pragma once

#include <SDL/SDL.h>

#include "gui.h"

struct GameData;

struct Velocity
{
	float x = 0.0f;
	float y = 0.0f;
};

struct Paddle
{
	Texture texture = {};
	Velocity velocity = {};
};

struct Ball
{
	Texture texture = {};
	Velocity velocity = {};

	float rotationAngle = 0.0f;
};

struct Brick
{
	int numOfHitsLeft = 1;

	Texture texture = {};
	Velocity velocity = {};
};

void ballReset(SDL_Renderer* renderer, Ball& ball, const char* filepath = "res/balls/default_ball.png");
bool ballUpdate(GameData& gameData, Ball& ball);

void paddleReset(SDL_Renderer* renderer, Paddle& paddle);
void paddleUpdate(GameData& gameData, Paddle& paddle);

void bricksReset(GameData& gameData);
