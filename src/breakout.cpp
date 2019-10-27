#include <stdio.h>
#include <chrono>
#include <math.h>

#include <SDL/SDL.h>

#include "breakout.h"
#include "entity.h"

// NOTE(fkp): Returns true on success, false on fail
GameData init()
{
	GameData result = {};
	result.keyboardState = SDL_GetKeyboardState(NULL);
	result.running = true;  // Set to false if init fails
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		printf("Failed to Initialise SDL2\n");
		result.running = false;
	}

	if (TTF_Init() != 0)
	{
		printf("Failed to initialise TTF\n");
		result.running = false;
	}

	result.window = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (result.window == nullptr)
	{
		printf("Failed to Create Window\n");
		result.running = false;
	}

	result.renderer = SDL_CreateRenderer(result.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (result.renderer == nullptr)
	{
		printf("Failed to Create Renderer\n");
		result.running = false;
	}

	return result;
}

// TODO(lucky962): Should this be separate for each game state?
bool gameInit(GameData& gameData)
{
	// Text init
	gameData.fpsText.text = "Breakout V0.1.0 | 0.00 FPS";
	gameData.fpsText.rect.x = SCREEN_WIDTH * 4 / 5;
	gameData.fpsText.rect.y = SCREEN_HEIGHT * 15 / 16;
	updateTextTexture(gameData.renderer, ARIAL_FONT_PATH, gameData.fpsText);
	
	// TEST
	gameData.testTexture = createTexture(gameData.renderer, "res/test-texture.png");
	
	paddleReset(gameData.paddle);
	ballReset(gameData.ball);

	int brickX = SCREEN_WIDTH / 24;
	int brickY = SCREEN_HEIGHT / 10;

	for (Entity& brick : gameData.bricks)
	{
		brick.colour = SDL_Color { 255, 255, 255, 255 };
		brick.rect = { brickX, brickY, BRICK_WIDTH, BRICK_HEIGHT };

		brickX += BRICK_WIDTH + (SCREEN_WIDTH / 12);
	}

	return true;
}

// NOTE(fkp): Returns true if success, false if games needs to exit
bool gameplayHandleEvents(GameData& gameData)
{
	while (SDL_PollEvent(&gameData.event))
	{
		switch (gameData.event.type)
		{
			case SDL_QUIT:
			{
				return false;
			} break;
		}
	}

	gameData.paddle.velocity.x = 0;

	if (gameData.keyboardState[SDL_SCANCODE_RIGHT])
	{
		gameData.paddle.velocity.x += PADDLE_VELOCITY;
	}

	if (gameData.keyboardState[SDL_SCANCODE_LEFT])
	{
		gameData.paddle.velocity.x -= PADDLE_VELOCITY;
	}

	return true;
}

// NOTE(fkp): Returns true if success, false if games needs to exit
bool gameplayUpdate(GameData& gameData)
{
	// Left/right bounds checking for paddle
	if (gameData.paddle.rect.x < 0)
	{
		gameData.paddle.rect.x = 0;
	}

	if (gameData.paddle.rect.x + PADDLE_WIDTH > SCREEN_WIDTH)
	{
		gameData.paddle.rect.x = SCREEN_WIDTH - PADDLE_WIDTH;
	}
	
	// Ball Bouncing off side of window
	if (gameData.ball.rect.x < 0)
	{
		gameData.ball.rect.x = 0;
		gameData.ball.velocity.x *= -1;
	}
	else if (gameData.ball.rect.x + BALL_WIDTH > SCREEN_WIDTH)
	{
		gameData.ball.rect.x = SCREEN_WIDTH - BALL_WIDTH;
		gameData.ball.velocity.x *= -1;
	}
	
	if (gameData.ball.rect.y < 0)
	{
		gameData.ball.rect.y = 0;
		gameData.ball.velocity.y *= -1;
	}

	if (gameData.ball.rect.y + BALL_HEIGHT > SCREEN_HEIGHT)
	{
		// NOTE(fkp): Game over
		// TODO(fkp): Splash screen instead of just restarting ball
		ballReset(gameData.ball);
		paddleReset(gameData.paddle);
	}

	if (SDL_HasIntersection(&gameData.ball.rect, &gameData.paddle.rect) == SDL_TRUE)
	{
		const float maxBallBounceAngle = 60;
		float ballCenterX = gameData.ball.rect.x + (BALL_WIDTH / 2.0f);
		float paddleCenterX = gameData.paddle.rect.x + (PADDLE_WIDTH / 2.0f);
		float maxTouchingDistance = (PADDLE_WIDTH / 2.0f) + (BALL_WIDTH / 2.0f);
		float ballAngle = (((ballCenterX - paddleCenterX) / maxTouchingDistance) * maxBallBounceAngle) + 90;
		gameData.ball.velocity.x = (int) (-cos(ballAngle * PI / 180) * BALL_VELOCITY);
		gameData.ball.velocity.y = (int) (-sin(ballAngle * PI / 180) * BALL_VELOCITY);
	}

	// Moves ball on x-axis and checks for collision
	gameData.ball.rect.x += (int) (gameData.ball.velocity.x * gameData.deltaTime);

	for (Entity& brick : gameData.bricks)
	{
		if (SDL_HasIntersection(&gameData.ball.rect, &brick.rect))
		{
			// Moves back outside the rect
			gameData.ball.rect.x -= (int) (gameData.ball.velocity.x * gameData.deltaTime);
			// Switches x-axis direction
			gameData.ball.velocity.x *= -1;
		}
	}

	// Moves ball on y-axis and checks for collision
	gameData.ball.rect.y += (int) (gameData.ball.velocity.y * gameData.deltaTime);

	for (Entity& brick : gameData.bricks)
	{
		if (SDL_HasIntersection(&gameData.ball.rect, &brick.rect))
		{
			// Moves back outside the rect
			gameData.ball.rect.y -= (int) (gameData.ball.velocity.y * gameData.deltaTime);
			// Switches y-axis direction
			gameData.ball.velocity.y *= -1;
		}
	}
	
	gameData.paddle.rect.x += (int) (gameData.paddle.velocity.x * gameData.deltaTime);

	return true;
}

// Draws the game state
void gameplayDraw(GameData& gameData)
{
	SDL_SetRenderDrawColor(gameData.renderer, 0, 0, 0, 255);
	SDL_RenderClear(gameData.renderer);

	SDL_Color colour;
	
	colour = gameData.paddle.colour;
	SDL_SetRenderDrawColor(gameData.renderer, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderFillRect(gameData.renderer, &gameData.paddle.rect);

	for (Entity& brick : gameData.bricks)
	{
		colour = brick.colour;
		SDL_SetRenderDrawColor(gameData.renderer, colour.r, colour.g, colour.b, colour.a);
		SDL_RenderFillRect(gameData.renderer, &brick.rect);
	}
	
	colour = gameData.ball.colour;
	SDL_SetRenderDrawColor(gameData.renderer, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderFillRect(gameData.renderer, &gameData.ball.rect);

	SDL_RenderCopy(gameData.renderer, gameData.fpsText.texture, nullptr, &gameData.fpsText.rect);

	// TEST
	SDL_RenderCopy(gameData.renderer, gameData.testTexture.texture, nullptr, &gameData.testTexture.rect);

	SDL_RenderPresent(gameData.renderer);
}

int main(int argc, char* argv[])
{
	GameData gameData = init();
	gameInit(gameData);

	unsigned int frameCounter = 0;
	std::chrono::high_resolution_clock::time_point lastTime = std::chrono::high_resolution_clock::now();

	while (gameData.running)
	{
		// Calculates delta time
		std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> diff = currentTime - lastTime;
		gameData.deltaTime = diff.count() / 1000.0;
		lastTime = currentTime;

		// Text for frame rate
		frameCounter += 1;
		if (frameCounter % 20 == 0)
		{
			char newFpsText[256];
			sprintf_s(newFpsText, 256, "Breakout V0.1.0 | %dFPS", (int) (1.0 / gameData.deltaTime));
			gameData.fpsText.text = newFpsText;
			updateTextTexture(gameData.renderer, gameData.fpsText);

			frameCounter = 0;
		}

		switch (gameData.gameState)
		{
			case GameState::Gameplay: 
			{
				gameData.running = gameplayHandleEvents(gameData);
				if (gameData.running) gameData.running = gameplayUpdate(gameData);
				gameplayDraw(gameData);
			} break;
		}
	}
	
	return 0;
}
