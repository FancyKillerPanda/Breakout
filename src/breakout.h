#pragma once

#include <array>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "entity.h"
#include "gui.h"
#include "constants.h"

enum class GameState
{
	MainMenu,
	Gameplay,
};

struct GameData
{
	bool running = false;
	bool fullscreen = false;
	bool paused = false;

	// NOTE(fkp): Delta-time is in seconds
	double deltaTime = 0.0;
	
	// SDL2 data
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Event event = {};
	const uint8_t* keyboardState = nullptr;

	// TODO(lucky962): Start Screen
	GameState gameState = GameState::MainMenu;

	// Entities data
	Paddle paddle = {};
	Ball ball = {};
	std::array<Brick, NUM_BRICKS_X_AXIS * NUM_BRICKS_Y_AXIS> bricks = {};

	// Text data
	Text fpsText = {};
	Text pausedText = {};
	Text menus[NUM_ITEMS_IN_MENU] = {};
	bool selected[NUM_ITEMS_IN_MENU] = {0, 0, 0};
};
