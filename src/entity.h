#pragma once

#include <SDL/SDL.h>

struct Velocity
{
	int x = 0;
	int y = 0;
};

struct Entity
{
	SDL_Rect rect = {};
	SDL_Color colour = {};
	Velocity velocity = {};
};

void ballReset(Entity& ball);
void paddleReset(Entity& paddle);
