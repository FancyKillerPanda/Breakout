#pragma once

#include <SDL/SDL.h>

#include "gui.h"

struct Velocity
{
	int x = 0;
	int y = 0;
};

struct Entity
{
	Texture texture = {};
	SDL_Color colour = {};
	Velocity velocity = {};
};

void ballReset(SDL_Renderer* renderer, Entity& ball);
void paddleReset(SDL_Renderer* renderer, Entity& paddle);
