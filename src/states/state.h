#pragma once

#include "breakout.h"

// NOTE(fkp): Returns true if success, false if games needs to exit
bool gameplayHandleEvents(GameData& gameData);
// NOTE(fkp): Returns true if success, false if games needs to exit
bool gameplayUpdate(GameData& gameData);
// Draws the game state
void gameplayDraw(GameData& gameData);

enum class MenuButtonSelected
{
	None,
	Start,
	Settings,
	Exit
};

// Initialises menu
void initMenu(GameData& gameData);
MenuButtonSelected menuHandleEvents(GameData& gameData);
// Draws menu
void menuDraw(GameData& gameData);

// Draws the gameover state
void gameoverDraw(GameData& gameData);