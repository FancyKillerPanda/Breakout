#include <stdio.h>
#include <chrono>
#include <math.h>

#include <SDL/SDL.h>

#define PI 3.14159265

constexpr int SCREEN_WIDTH = 960;
constexpr int SCREEN_HEIGHT = 540;

constexpr int PADDLE_WIDTH = SCREEN_WIDTH / 6;
constexpr int PADDLE_HEIGHT = SCREEN_HEIGHT / 30;
constexpr int PADDLE_VELOCITY = SCREEN_WIDTH / 2;

constexpr int BALL_WIDTH = SCREEN_WIDTH / 50;
constexpr int BALL_HEIGHT = BALL_WIDTH;
constexpr int BALL_VELOCITY = SCREEN_WIDTH / 2;

struct Velocity
{
	int x = 0;
	int y = 0;
};

struct Entity
{
	SDL_Rect rect = {};
	SDL_Color colour = {};
	Velocity velocity = {};
};

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
	gameState.paddle.colour = SDL_Colour { 255, 255, 255, 255 };
	gameState.paddle.rect = SDL_Rect { (SCREEN_WIDTH - PADDLE_WIDTH) / 2, (SCREEN_HEIGHT * 9 / 10) - (PADDLE_HEIGHT / 2), PADDLE_WIDTH, PADDLE_HEIGHT };

	gameState.ball.colour = SDL_Colour { 255, 255, 255, 255 };
	gameState.ball.rect = SDL_Rect { (SCREEN_WIDTH - BALL_WIDTH) / 2, (SCREEN_HEIGHT  - BALL_HEIGHT) / 2, BALL_WIDTH, BALL_HEIGHT };
	gameState.ball.velocity.y = BALL_VELOCITY;

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
		gameState.ball.velocity.x = -gameState.ball.velocity.x;
	}
	
	if (gameState.ball.rect.y < 0)
	{
		gameState.ball.velocity.y = -gameState.ball.velocity.y;
	}

	if (gameState.ball.rect.y + BALL_HEIGHT > SCREEN_HEIGHT)
	{
		gameState.ball.velocity.x = 0;
		gameState.ball.velocity.y = 0;
	}

	if (SDL_HasIntersection(&gameState.ball.rect, &gameState.paddle.rect) == SDL_TRUE)
	{
		// TODO(fkp): Clean this up
		// TODO(fkp): Angle should always be between 0 and 180
		float ball_angle = (float)((((((float) gameState.ball.rect.x + (float) BALL_WIDTH / 2.0) - ((float) gameState.paddle.rect.x + (float) PADDLE_WIDTH / 2.0)) / ((float) PADDLE_WIDTH / 2.0)) * 90.0) + 90.0);
		gameState.ball.velocity.x = (int) (-cos(ball_angle * PI / 180) * BALL_VELOCITY);
		gameState.ball.velocity.y = (int) (-sin(ball_angle * PI / 180) * BALL_VELOCITY);
		printf("%f", ball_angle);
	}
	
	gameState.paddle.rect.x += (int) (gameState.paddle.velocity.x * gameState.deltaTime);
	gameState.ball.rect.x += (int) (gameState.ball.velocity.x * gameState.deltaTime);
	gameState.ball.rect.y += (int) (gameState.ball.velocity.y * gameState.deltaTime);

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
