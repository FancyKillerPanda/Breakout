#pragma once

#include <array>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "entity.h"
#include "gui.h"
#include "vector"

enum class GameState
{
	Gameplay,
};

struct GameData
{
	bool running = false;
	bool fullscreen = false;

	// NOTE(fkp): Delta-time is in seconds
	double deltaTime = 0.0;
	
	// SDL2 data
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Event event = {};
	const uint8_t* keyboardState = nullptr;

	// TODO(lucky962): Start Screen
	GameState gameState = GameState::Gameplay;

	// Entities data
	Paddle paddle = {};
	Ball ball = {};
	std::array<Brick, 4> bricks = {};

	// Text data
	Text fpsText = {};

	// Textures
};
