#pragma once

#include <SDL/SDL.h>

#include "gui.h"

struct Velocity
{
	float x = 0.0f;
	float y = 0.0f;
};

struct Entity
{
	Texture texture = {};
	SDL_Color colour = {};
	Velocity velocity = {};
};

void ballReset(SDL_Renderer* renderer, Entity& ball);
void paddleReset(SDL_Renderer* renderer, Entity& paddle);
