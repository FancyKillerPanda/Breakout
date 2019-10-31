#pragma once

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "constants.h"

struct Texture
{
	const char* filepath = "";
	
	SDL_Texture* texture = nullptr;
	SDL_Rect rect = {};
};

Texture createTexture(SDL_Renderer* renderer, const char* filepath);
void drawTexture(SDL_Renderer* renderer, Texture& texture);
void drawTexture(SDL_Renderer* renderer, Texture& texture, double angle);
void destroyTexture(Texture& texture);

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
void drawText(SDL_Renderer* renderer, Text& text);
