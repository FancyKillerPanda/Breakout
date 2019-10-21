#include <stdio.h>

#include <SDL/SDL.h>


int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		printf("It does not work.");
		return -1;
	}

	printf("It works.");
	return 0;
}
