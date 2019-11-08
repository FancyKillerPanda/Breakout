#pragma once

#include <array>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "entity.h"
#include "gui.h"
#include "constants.h"
#include "settings.h"

enum class GameState
{
	MainMenu,
	Gameplay,
	GameOver,
};

enum class MainMenuState
{
	Home,
	Customise,
};

struct MainMenuData
{
	MainMenuState state;

	// Home page menu items
	Menu homePageMenu = {};

	// Texture for highlighting
	Texture circleHighlight = {};

	// Arrows
	Texture ballLeftArrow = {};
	bool ballLeftArrowSelected = false;
	Texture ballRightArrow = {};
	bool ballRightArrowSelected = false;

	// Back button
	bool backButtonSelected = false;
	Text backButton = {};

	// Ball customisation
	int ballCurrentlySelectedIndex = 0;
	Text ballsText = {};
	int ballInViewIndex = 0;
	std::array<Ball, MENU_CUSTOMISE_NUMBER_OF_BALLS> balls = {};
};

struct GameOverData
{
	Text gameOverText = {};
	Menu gameOverMenu = {};
};

struct GameData
{
	bool running = false;
	bool fullscreen = false;
	bool paused = false;

	bool mainMenuInitialised = false;
	bool gameplayInitialised = false;
	bool gameOverInitialised = false;

	Settings settings;

	// NOTE(fkp): Delta-time is in seconds
	double deltaTime = 0.0;
	
	// SDL2 data
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Event event = {};
	const uint8_t* keyboardState = nullptr;

	// State data
	GameState gameState = GameState::MainMenu;
	MainMenuData mainMenuData = {};
	GameOverData gameOverData = {};

	// Entities data
	Paddle paddle = {};
	Ball ball = {};
	std::array<Brick, NUM_BRICKS_X_AXIS * NUM_BRICKS_Y_AXIS> bricks = {};

	// Text data
	Text fpsText = {};
	Text pausedText = {};
};
