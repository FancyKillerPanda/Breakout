#include <stdio.h>

#include "text.h"

void updateTextTexture(SDL_Renderer* renderer, Text& text)
{
	// Destroys the last texture if there was one
	SDL_DestroyTexture(text.texture);
	text.texture = nullptr;

	if (text.font == nullptr)
	{
		printf("Text requires valid font (text: %s)\n", text.text);
		return;
	}

	// Creates a surface for the text
	SDL_Surface* textSurface = TTF_RenderUTF8_Solid(text.font, text.text, text.colour);

	if (textSurface == nullptr)
	{
		printf("Text surface creation failed (text: %s)\n%s\n", text.text, SDL_GetError());
		return;
	}

	// Converts surface to texture
	text.texture = SDL_CreateTextureFromSurface(renderer, textSurface);

	if (text.texture == nullptr)
	{
		printf("Text surface to texture failed (text: %s)\n%s\n", text.text, SDL_GetError());
		return;
	}

	// Frees temporary surface
	SDL_FreeSurface(textSurface);

	// Gets texture dimensions
	SDL_QueryTexture(text.texture, nullptr, nullptr, &text.rect.w, &text.rect.h);
}

void updateTextTexture(SDL_Renderer* renderer, const char* fontPath, Text& text)
{
	// Destroys old font if necessary
	if (text.font)
	{
		TTF_CloseFont(text.font);
	}
	
	text.font = TTF_OpenFont(fontPath, text.size);

	if (text.font == nullptr)
	{
		printf("Font creation failed (font: %s)\n%s\n", fontPath, SDL_GetError());
		return;
	}

	updateTextTexture(renderer, text);
}
