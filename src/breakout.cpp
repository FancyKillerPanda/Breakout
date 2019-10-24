#include <stdio.h>
#include <chrono>
#include <math.h>

#include <SDL/SDL.h>

#include "breakout.h"
#include "entity.h"

// NOTE(fkp): Returns true on success, false on fail
GameState init()
{
	GameState result = {};
	result.keyboardState = SDL_GetKeyboardState(NULL);
	result.running = true;  // Set to false if init fails
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		printf("Failed to Initialise SDL2");
		result.running = false;
	}

	result.window = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (result.window == nullptr)
	{
		printf("Failed to Create Window");
		result.running = false;
	}

	result.renderer = SDL_CreateRenderer(result.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (result.renderer == nullptr)
	{
		printf("Failed to Create Renderer");
		result.running = false;
	}

	return result;
}

bool gameInit(GameState& gameState)
{
	paddleReset(gameState.paddle);
	ballReset(gameState.ball);

	int brickX = SCREEN_WIDTH / 24;
	int brickY = SCREEN_HEIGHT / 10;

	for (Entity& brick : gameState.bricks)
	{
		brick.colour = SDL_Color { 255, 255, 255, 255 };
		brick.rect = { brickX, brickY, BRICK_WIDTH, BRICK_HEIGHT };

		brickX += BRICK_WIDTH + (SCREEN_WIDTH / 12);
	}

	return true;
}

// NOTE(fkp): Returns true if success, false if games needs to exit
bool gameHandleEvents(GameState& gameState)
{
	while (SDL_PollEvent(&gameState.event))
	{
		switch (gameState.event.type)
		{
			case SDL_QUIT:
			{
				return false;
			} break;
		}
	}

	gameState.paddle.velocity.x = 0;

	if (gameState.keyboardState[SDL_SCANCODE_RIGHT])
	{
		gameState.paddle.velocity.x += PADDLE_VELOCITY;
	}

	if (gameState.keyboardState[SDL_SCANCODE_LEFT])
	{
		gameState.paddle.velocity.x -= PADDLE_VELOCITY;
	}

	return true;
}

// NOTE(fkp): Returns true if success, false if games needs to exit
bool gameUpdate(GameState& gameState)
{
	// Left/right bounds checking for paddle
	if (gameState.paddle.rect.x < 0)
	{
		gameState.paddle.rect.x = 0;
	}

	if (gameState.paddle.rect.x + PADDLE_WIDTH > SCREEN_WIDTH)
	{
		gameState.paddle.rect.x = SCREEN_WIDTH - PADDLE_WIDTH;
	}
	
	// Ball Bouncing off side of window
	if ((gameState.ball.rect.x < 0) || (gameState.ball.rect.x + BALL_WIDTH > SCREEN_WIDTH))
	{
		gameState.ball.velocity.x *= -1;
	}
	
	if (gameState.ball.rect.y < 0)
	{
		gameState.ball.velocity.y *= -1;
	}

	if (gameState.ball.rect.y + BALL_HEIGHT > SCREEN_HEIGHT)
	{
		// NOTE(fkp): Game over
		// TODO(fkp): Splash screen instead of just restarting ball
		ballReset(gameState.ball);
		paddleReset(gameState.paddle);
	}

	if (SDL_HasIntersection(&gameState.ball.rect, &gameState.paddle.rect) == SDL_TRUE)
	{
		const float maxBallBounceAngle = 60;
		float ballCenterX = gameState.ball.rect.x + (BALL_WIDTH / 2.0f);
		float paddleCenterX = gameState.paddle.rect.x + (PADDLE_WIDTH / 2.0f);
		float maxTouchingDistance = (PADDLE_WIDTH / 2.0f) + (BALL_WIDTH / 2.0f);
		float ballAngle = (((ballCenterX - paddleCenterX) / maxTouchingDistance) * maxBallBounceAngle) + 90;
		gameState.ball.velocity.x = (int) (-cos(ballAngle * PI / 180) * BALL_VELOCITY);
		gameState.ball.velocity.y = (int) (-sin(ballAngle * PI / 180) * BALL_VELOCITY);
	}

	// Moves ball on x-axis and checks for collision
	gameState.ball.rect.x += (int) (gameState.ball.velocity.x * gameState.deltaTime);

	for (Entity& brick : gameState.bricks)
	{
		if (SDL_HasIntersection(&gameState.ball.rect, &brick.rect))
		{
			// Moves back outside the rect
			gameState.ball.rect.x -= (int) (gameState.ball.velocity.x * gameState.deltaTime);
			// Switches x-axis direction
			gameState.ball.velocity.x *= -1;
		}
	}

	// Moves ball on y-axis and checks for collision
	gameState.ball.rect.y += (int) (gameState.ball.velocity.y * gameState.deltaTime);

	for (Entity& brick : gameState.bricks)
	{
		if (SDL_HasIntersection(&gameState.ball.rect, &brick.rect))
		{
			// Moves back outside the rect
			gameState.ball.rect.y -= (int) (gameState.ball.velocity.y * gameState.deltaTime);
			// Switches y-axis direction
			gameState.ball.velocity.y *= -1;
		}
	}
	
	gameState.paddle.rect.x += (int) (gameState.paddle.velocity.x * gameState.deltaTime);

	return true;
}

// Draws the game state
void gameDraw(GameState& gameState)
{
	SDL_SetRenderDrawColor(gameState.renderer, 0, 0, 0, 255);
	SDL_RenderClear(gameState.renderer);

	SDL_Color colour;
	
	colour = gameState.paddle.colour;
	SDL_SetRenderDrawColor(gameState.renderer, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderFillRect(gameState.renderer, &gameState.paddle.rect);

	for (Entity& brick : gameState.bricks)
	{
		colour = brick.colour;
		SDL_SetRenderDrawColor(gameState.renderer, colour.r, colour.g, colour.b, colour.a);
		SDL_RenderFillRect(gameState.renderer, &brick.rect);
	}
	
	colour = gameState.ball.colour;
	SDL_SetRenderDrawColor(gameState.renderer, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderFillRect(gameState.renderer, &gameState.ball.rect);

	SDL_RenderPresent(gameState.renderer);
}

int main(int argc, char* argv[])
{
	GameState gameState = init();
	gameInit(gameState);

	std::chrono::high_resolution_clock::time_point lastTime = std::chrono::high_resolution_clock::now();

	while (gameState.running)
	{
		// Calculates deltaTime
		std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> diff = currentTime - lastTime;
		gameState.deltaTime = diff.count() / 1000.0;
		lastTime = currentTime;

		gameState.running = gameHandleEvents(gameState);
		if (gameState.running) gameState.running = gameUpdate(gameState);
		gameDraw(gameState);
	}
	
	return 0;
}
