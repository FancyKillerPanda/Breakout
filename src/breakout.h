#pragma once

#include <SDL/SDL.h>

#include "entity.h"

constexpr double PI = 3.14159265358979323846;

constexpr int SCREEN_WIDTH = 960;
constexpr int SCREEN_HEIGHT = 540;

constexpr int PADDLE_WIDTH = SCREEN_WIDTH / 6;
constexpr int PADDLE_HEIGHT = SCREEN_HEIGHT / 30;
constexpr int PADDLE_VELOCITY = SCREEN_WIDTH / 2;

constexpr int BALL_WIDTH = SCREEN_WIDTH / 50;
constexpr int BALL_HEIGHT = BALL_WIDTH;
constexpr int BALL_VELOCITY = SCREEN_WIDTH / 2;

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
};
