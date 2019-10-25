#pragma once

#include <array>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "entity.h"
#include "gui.h"

struct GameState
{
	bool running = false;

	// NOTE(fkp): Delta-time is in seconds
	double deltaTime = 0.0;
	
	// SDL2 data
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Event event = {};
	const uint8_t* keyboardState = nullptr;
	
	// Entities data
	Entity paddle = {};
	Entity ball = {};
	std::array<Entity, 4> bricks = {};

	// Text data
	Text fpsText = {};
};
