#include <stdio.h>

#include "gui.h"

Texture createTexture(SDL_Renderer* renderer, const char* filepath)
{
	Texture result;
	result.filepath = filepath;
	result.texture = IMG_LoadTexture(renderer, result.filepath);

	if (result.texture == nullptr)
	{
		printf("Texture creation failed (filepath: %s)\n%s\n", result.filepath, SDL_GetError());
	}

	// Checks for valid texture dimensions
	if (SDL_QueryTexture(result.texture, nullptr, nullptr, &result.rect.w, &result.rect.h) == -1)
	{
		printf("Texture is invalid (filepath: %s)\n%s\n", result.filepath, SDL_GetError());
	}

	return result;
}

// NOTE(fkp): Do not need to call at the very end of the program
// Only necessary to call if textures will be created/destroyed
// multiple times during a program.
// Windows will clean up memory at the end of the program
void destroyTexture(Texture& texture)
{
	SDL_DestroyTexture(texture.texture);
	texture.texture = nullptr;
	texture.filepath = "";
}

void updateTextTexture(SDL_Renderer* renderer, Text& text)
{
	// Destroys the last texture if there was one
	SDL_DestroyTexture(text.texture);
	text.texture = nullptr;

	if (text.font == nullptr)
	{
		printf("Text requires valid font (text: %s)\n", text.text.c_str());
		return;
	}

	// Creates a surface for the text
	SDL_Surface* textSurface = TTF_RenderUTF8_Solid(text.font, text.text.c_str(), text.colour);

	if (textSurface == nullptr)
	{
		printf("Text surface creation failed (text: %s)\n%s\n", text.text.c_str(), SDL_GetError());
		return;
	}

	// Converts surface to texture
	text.texture = SDL_CreateTextureFromSurface(renderer, textSurface);

	if (text.texture == nullptr)
	{
		printf("Text surface to texture failed (text: %s)\n%s\n", text.text.c_str(), SDL_GetError());
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

Text createText(SDL_Renderer* renderer, std::string text, int size, int centerX, int centerY)
{
	return createText(renderer, text, size, centerX, centerY, DIGITAL_DISCO_FONT_PATH, SDL_Color { 255, 255, 255, 255 });
}

Text createText(SDL_Renderer* renderer, std::string text, int size, int centerX, int centerY, const char* fontPath, SDL_Color colour)
{
	Text result;
	result.text = text;
	result.size = size;
	result.colour = colour;
	updateTextTexture(renderer, fontPath, result);
	result.rect.x = centerX - (result.rect.w / 2);
	result.rect.y = centerY - (result.rect.h / 2);

	return result;
}

Menu menuConstruct(SDL_Renderer* renderer, std::vector<std::string> texts, std::vector<std::pair<int, int>> positions)
{
	Menu result = {};

	if (texts.size() != positions.size())
	{
		printf("Error: Menu construct text size not equal position size.\n");
		return result;
	}
	
	for (int i = 0; i < texts.size(); i++)
	{
		result.items.push_back(createText(renderer, texts[i], 30, positions[i].first, positions[i].second, DIGITAL_DISCO_FONT_PATH, MENU_COLOURS[0]));
	}

	result.items[0].colour = MENU_COLOURS[1];
	updateTextTexture(renderer, DIGITAL_DISCO_FONT_PATH, result.items[0]);
	result.itemSelected = 0;

	return result;
}

void menuHandleMouseMove(const GameData& gameData, Menu& menu)
{
	SDL_Point mousePos = { gameData.event.motion.x, gameData.event.motion.y };

	// Removes old selection
	if ((menu.itemSelected != -1) && (!SDL_PointInRect(&mousePos, &menu.items[menu.itemSelected].rect)))
	{
		menu.items[menu.itemSelected].colour = MENU_COLOURS[0];
		updateTextTexture(gameData.renderer, menu.items[menu.itemSelected]);
		menu.itemSelected = -1;
	}

	for (int i = 0; i < menu.items.size(); i++)
	{
		if ((menu.itemSelected != i) && (SDL_PointInRect(&mousePos, &menu.items[i].rect)))
		{
			// Highlights new selection
			menu.itemSelected = i;
			menu.items[i].colour = MENU_COLOURS[1];
			updateTextTexture(gameData.renderer, menu.items[i]);
		}
	}
}

int menuHandlePress(const Menu& menu)
{
	return menu.itemSelected;
}

void menuHandleKeyDown(const GameData& gameData, Menu& menu)
{
	if (menu.itemSelected != -1)
	{
		menu.items[menu.itemSelected].colour = MENU_COLOURS[0];
		updateTextTexture(gameData.renderer, menu.items[menu.itemSelected]);
	}

	if (gameData.event.key.keysym.sym == SDLK_RIGHT)
	{
		menu.itemSelected += 1;

		if (menu.itemSelected >= menu.items.size())
		{
			menu.itemSelected = 0;
		}
	}
	else if (gameData.event.key.keysym.sym == SDLK_LEFT)
	{
		menu.itemSelected -= 1;

		if (menu.itemSelected < 0)
		{
			menu.itemSelected = (int) (menu.items.size() - 1);
		}
	}
	else
	{
		// NOTE(fkp): Should be unreachable
		printf("Reached unreachable location (menuHandleKeyDown).");
	}

	menu.items[menu.itemSelected].colour = MENU_COLOURS[1];
	updateTextTexture(gameData.renderer, menu.items[menu.itemSelected]);
}

void menuDraw(SDL_Renderer* renderer, Menu& menu)
{
	for (int i = 0; i < menu.items.size(); i++)
	{
		drawText(renderer, menu.items[i]);
	}
}
