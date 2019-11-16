#pragma once

#include <utility>
#include <vector>
#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "constants.h"

struct GameData;

struct Texture
{
	const char* filepath = "";
	
	SDL_Texture* texture = nullptr;
	SDL_Rect rect = {};
};

Texture createTexture(SDL_Renderer* renderer, const char* filepath);
void destroyTexture(Texture& texture);

inline void drawTexture(SDL_Renderer* renderer, Texture& texture, double angle = 0.0, SDL_RendererFlip flip = SDL_FLIP_NONE)
{
	SDL_RenderCopyEx(renderer, texture.texture, nullptr, &texture.rect, angle, nullptr, flip);
}

struct Text
{
	TTF_Font* font = nullptr;
	
	SDL_Texture* texture = nullptr;
	SDL_Rect rect = {};
	
	std::string text = "";
	unsigned int size = 14;
	SDL_Color colour = SDL_Color { 255, 255, 255, 255 };
};

Text createText(SDL_Renderer* renderer, std::string text, int size, int centerX, int centerY);
Text createText(SDL_Renderer* renderer, std::string text, int size, int centerX, int centerY, const char* fontPath, SDL_Color colour);
void updateTextTexture(SDL_Renderer* renderer, Text& text);
void updateTextTexture(SDL_Renderer* renderer, const char* fontPath, Text& text);

inline void drawText(SDL_Renderer* renderer, Text& text)
{
	SDL_RenderCopy(renderer, text.texture, nullptr, &text.rect);
}

struct Menu
{
	// Menu items
	std::vector<Text> items = {};
	int itemSelected = 0;
};


Menu menuConstruct(SDL_Renderer* renderer, std::vector<std::string> texts, std::vector<std::pair<int, int>> positions);
void menuHandleMouseMove(const GameData& gameData, Menu& menu);
int menuHandlePress(const Menu& menu);
void menuHandleKeyDown(const GameData& gameData, Menu& menu);
void menuDraw(SDL_Renderer* renderer, Menu& menu);
