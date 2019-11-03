#pragma once

#include "breakout.h"

// Initialises gameplay
void gameplayInit(GameData& gameData);
// Called each time entering the state
void gameplayOnEnter(GameData& gameData);
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
	Customise,
	Exit
};

// Initialises menu
void menuInit(GameData& gameData);
// Called each time entering the state
void menuOnEnter(GameData& gameData);
MenuButtonSelected menuHandleEvents(GameData& gameData);
// Draws menu
void menuDraw(GameData& gameData);

// Updates the locations and sizes of the customisable balls
void updateSelectedBall(GameData& gameData);

// NOTE(fkp): Only call this to change states
inline void changeState(GameData& gameData, GameState newState)
{
	gameData.gameState = newState;

	// TODO(fkp): Maybe have an onExit function?
	switch (newState)
	{
		case GameState::Gameplay:
		{
			gameplayInit(gameData); // Will not do anything if already initialised
			gameplayOnEnter(gameData);
		} break;
		case GameState::Menu:
		{
			menuInit(gameData); // Will not do anything if already initialised
			menuOnEnter(gameData);
		} break;
	}
}
