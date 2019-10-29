#pragma once

#include <SDL/SDL.h>

#include "gui.h"

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
};

struct Brick
{
	Texture texture = {};
	Velocity velocity = {};

	int numOfHitsLeft = 1;
};

void ballReset(SDL_Renderer* renderer, Ball& ball);
void paddleReset(SDL_Renderer* renderer, Paddle& paddle);
