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

// Initialises main menu
void mainMenuInit(GameData& gameData);
// Called each time entering the state
void mainMenuOnEnter(GameData& gameData);
MenuButtonSelected menuHandleEvents(GameData& gameData);
// Draws main menu
void mainMenuDraw(GameData& gameData);

// Initialises game over menu
void gameOverInit(GameData& gameData);
void gameOverHandleEvents(GameData& gameData);
// Draws the gameover state
void gameOverDraw(GameData& gameData);

// Updates the locations and sizes of the customisable balls
void updateSelectedBall(GameData& gameData);
// Updates the highlighting of the left and right arrows
void updateArrowHighlighting(SDL_Renderer* renderer, const SDL_Point& mousePos, Texture& arrow, bool& arrowSelected);

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

		case GameState::MainMenu:
		{
			mainMenuInit(gameData); // Will not do anything if already initialised
			mainMenuOnEnter(gameData);
		} break;
		
		case GameState::GameOver:
		{
			gameOverInit(gameData);
		} break;
	}
}
