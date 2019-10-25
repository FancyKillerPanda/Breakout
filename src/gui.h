#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "constants.h"

struct Text
{
	TTF_Font* font = nullptr;
	
	SDL_Texture* texture = nullptr;
	SDL_Rect rect = {};
	
	char* text = "Text";
	unsigned int size = 14;
	SDL_Color colour = SDL_Color { 255, 0, 0, 255 };
};

void updateTextTexture(SDL_Renderer* renderer, Text& text);
void updateTextTexture(SDL_Renderer* renderer, const char* fontPath, Text& text);
