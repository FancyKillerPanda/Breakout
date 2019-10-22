#include <stdio.h>
#include <chrono>

#include <SDL/SDL.h>

constexpr int SCREEN_WIDTH = 960;
constexpr int SCREEN_HEIGHT = 540;

constexpr int PADDLE_WIDTH = SCREEN_WIDTH / 6;
constexpr int PADDLE_HEIGHT = SCREEN_HEIGHT / 30;
constexpr int PADDLE_VELOCITY = 10;

struct Paddle
{
	SDL_Rect rect = {};
	SDL_Color colour = {};
	int velocity = 0;
};

struct GameState
{
	bool running = false;
	
	// SDL2 data
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Event event = {};
	const uint8_t* keyboardState = nullptr;
	
	// Paddle data
	Paddle paddle = {};
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

	gameState.paddle.velocity = 0;

	if (gameState.keyboardState[SDL_SCANCODE_RIGHT])
	{
		gameState.paddle.velocity += PADDLE_VELOCITY;
	}

	if (gameState.keyboardState[SDL_SCANCODE_LEFT])
	{
		gameState.paddle.velocity -= PADDLE_VELOCITY;
	}

	return true;
}

// NOTE(fkp): Returns true if success, false if games needs to exit
// TODO(fkp): Take delta-time
// TODO(fkp): Potentially take game state in an object
bool gameUpdate(GameState& gameState)
{
	gameState.paddle.rect.x += gameState.paddle.velocity;

	return true;
}

// Draws the game state
// TODO(fkp): Potentially take game state in an object
void gameDraw(GameState& gameState)
{
	SDL_SetRenderDrawColor(gameState.renderer, 0, 0, 0, 255);
	SDL_RenderClear(gameState.renderer);

	SDL_Color colour = gameState.paddle.colour;
	SDL_SetRenderDrawColor(gameState.renderer, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderFillRect(gameState.renderer, &gameState.paddle.rect);

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
		double deltaTime = diff.count();
		lastTime = currentTime;

		// Test
		printf("%fms\n", deltaTime);
		
		gameState.running = gameHandleEvents(gameState);
		if (gameState.running) gameState.running = gameUpdate(gameState);
		gameDraw(gameState);
	}
	
	return 0;
}
