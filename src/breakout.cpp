#include <stdio.h>

#include <SDL/SDL.h>

constexpr int SCREEN_WIDTH = 960;
constexpr int SCREEN_HEIGHT = 540;

SDL_Window* g_Window = nullptr;
SDL_Renderer* g_Renderer = nullptr;

int init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		printf("It does not work.");
		return -1;
	}

	g_Window = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (g_Window == nullptr)
	{
		printf("Failed to Create Window");
		return -1;
	}

	g_Renderer = SDL_CreateRenderer(g_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (g_Renderer == nullptr)
	{
		printf("Failed to Create Renderer");
		return -1;
	}

	return 0;
}

int main(int argc, char* argv[])
{
	if (init() != 0)
	{
		return -1;
	}
	
	bool running = true;
	SDL_Event event = {};

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
				{
					running = false;
				} break;
			}
		}

		SDL_RenderClear(g_Renderer);
		SDL_RenderPresent(g_Renderer);
	}
	
	return 0;
}
