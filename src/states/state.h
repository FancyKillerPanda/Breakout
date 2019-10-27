#pragma once

#include "breakout.h"

// NOTE(fkp): Returns true if success, false if games needs to exit
bool gameplayHandleEvents(GameData& gameData);

// NOTE(fkp): Returns true if success, false if games needs to exit
bool gameplayUpdate(GameData& gameData);

// Draws the game state
void gameplayDraw(GameData& gameData);