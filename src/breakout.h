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
	Menu,
	Gameplay,
};

enum class MenuState
{
	Home,
	Customise,
};

struct MenuData
{
	MenuState state;

	// Menu items
	Text homeMenuItems[NUM_ITEMS_IN_MENU] = {};
	bool homeMenuItemSelected[NUM_ITEMS_IN_MENU] = { 0, 0, 0 };
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

	// State data
	GameState gameState = GameState::Menu;
	MenuData menuData = {};

	// Entities data
	Paddle paddle = {};
	Ball ball = {};
	std::array<Brick, NUM_BRICKS_X_AXIS * NUM_BRICKS_Y_AXIS> bricks = {};

	// Text data
	Text fpsText = {};
	Text pausedText = {};
};
