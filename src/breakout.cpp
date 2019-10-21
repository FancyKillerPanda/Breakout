#include <stdio.h>

#include <SDL/SDL.h>

constexpr int SCREEN_WIDTH = 960;
constexpr int SCREEN_HEIGHT = 540;

SDL_Window* g_Window = nullptr;
SDL_Renderer* g_Renderer = nullptr;

// NOTE: Returns true on success, false on fail
bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		printf("Failed to Initialise SDL2");
		return false;
	}

	g_Window = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (g_Window == nullptr)
	{
		printf("Failed to Create Window");
		return false;
	}

	g_Renderer = SDL_CreateRenderer(g_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (g_Renderer == nullptr)
	{
		printf("Failed to Create Renderer");
		return false;
	}

	return true;
}

// NOTE(fkp): Returns true if success, false if games needs to exit
bool gameHandleEvents()
{
	SDL_Event event = {};

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
			{
				return false;
			} break;
		}
	}

	return true;
}

// NOTE(fkp): Returns true if success, false if games needs to exit
// TODO(fkp): Take delta-time
// TODO(fkp): Potentially take game state in an object
bool gameUpdate()
{
	// TODO(fkp): Implement game update
	return true;
}

// Draws the game state
// TODO(fkp): Potentially take game state in an object
void gameDraw()
{
	SDL_RenderClear(g_Renderer);
	// Draw code goes here
	SDL_RenderPresent(g_Renderer);
}

int main(int argc, char* argv[])
{
	bool running = init();

	while (running)
	{
		running = gameHandleEvents();
		if (running) running = gameUpdate();
		gameDraw();
	}
	
	return 0;
}
