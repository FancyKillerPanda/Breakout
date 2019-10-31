#pragma once

#include "breakout.h"

// Initialises menu
void initMenu(GameData& gameData);

// NOTE(fkp): Returns true if success, false if games needs to exit
bool gameplayHandleEvents(GameData& gameData);

// NOTE(fkp): Returns true if success, false if games needs to exit
bool gameplayUpdate(GameData& gameData);

// NOTE(lucky962): Returns int based on menu button clicked. (0 = Nothing, 1 = Start, 2 = Settings, 3 = Exit)
int menuHandleEvents(GameData& gameData);

// Draws the game state
void gameplayDraw(GameData& gameData);
void menuDraw(GameData& gameData);
