#include <SDL/SDL.h>

#include "state.h"

void mainMenuInit(GameData& gameData)
{
	if (gameData.mainMenuInitialised)
	{
		return;
	}

	MainMenuData& mainMenuData = gameData.mainMenuData;

	// Back button
	mainMenuData.backButton.text = "Back";
	mainMenuData.backButton.size = 24;
	mainMenuData.backButton.colour = MENU_COLOURS[0];
	updateTextTexture(gameData.renderer, DIGITAL_DISCO_FONT_PATH, mainMenuData.backButton);
	mainMenuData.backButton.rect.x = SCREEN_WIDTH / 10;
	mainMenuData.backButton.rect.y = SCREEN_HEIGHT * 7 / 8;
	
	// Highlighting texture
	mainMenuData.circleHighlight = createTexture(gameData.renderer, "res/circle_highlight.png");
	
	// Home page init
	mainMenuData.title.text = "Breakout";
	mainMenuData.title.size = 80;
	mainMenuData.title.colour = MENU_COLOURS[0];
	updateTextTexture(gameData.renderer, DIGITAL_DISCO_FONT_PATH, mainMenuData.title);
	mainMenuData.title.rect.x = SCREEN_WIDTH / 2 - mainMenuData.title.rect.w / 2;
	mainMenuData.title.rect.y = SCREEN_HEIGHT / 3 - mainMenuData.title.rect.h / 2;
	std::vector<std::string> labels = { "START", "SETTINGS", "EXIT" };
	std::vector<std::pair<int, int>> positions = {
		{ SCREEN_WIDTH / 4,     SCREEN_HEIGHT / 8 * 7 },
		{ SCREEN_WIDTH / 4 * 2, SCREEN_HEIGHT / 8 * 7 }, 
		{ SCREEN_WIDTH / 4 * 3, SCREEN_HEIGHT / 8 * 7 } 
	};
	mainMenuData.homePageMenu = menuConstruct(gameData.renderer, labels, positions);

	// Customise page init
	mainMenuData.ballsText.colour = MENU_COLOURS[0];
	mainMenuData.ballsText.text = "Ball:";
	mainMenuData.ballsText.size = 30; // TODO(fkp): Extract font size into constant
	updateTextTexture(gameData.renderer, DIGITAL_DISCO_FONT_PATH, mainMenuData.ballsText);
	mainMenuData.ballsText.rect.x = MENU_CUSTOMISE_BALL_TEXT_X;
	mainMenuData.ballsText.rect.y = MENU_CUSTOMISE_BALL_TEXT_CENTER_Y - mainMenuData.ballsText.rect.h / 2;
	
	arrowReset(gameData.renderer, mainMenuData.ballLeftArrow, ARROW_TEXTURE_PATH);
	mainMenuData.ballLeftArrow.rect.x = MENU_CUSTOMISE_BALL_PREV_CENTER_X - (MENU_CUSTOMISE_BALL_NOT_IN_VIEW_WIDTH / 2) - 30 - mainMenuData.ballLeftArrow.rect.w;
	mainMenuData.ballLeftArrow.rect.y = MENU_CUSTOMISE_BALL_PREV_CENTER_Y - mainMenuData.ballLeftArrow.rect.h / 2;

	arrowReset(gameData.renderer, mainMenuData.ballRightArrow, ARROW_TEXTURE_PATH);
	mainMenuData.ballRightArrow.rect.x = MENU_CUSTOMISE_BALL_NEXT_CENTER_X + (MENU_CUSTOMISE_BALL_NOT_IN_VIEW_WIDTH / 2) + 30;
	mainMenuData.ballRightArrow.rect.y = MENU_CUSTOMISE_BALL_NEXT_CENTER_Y - mainMenuData.ballRightArrow.rect.h / 2;
	
	ballReset(gameData.renderer, mainMenuData.balls[0], "res/balls/default_ball.png");
	ballReset(gameData.renderer, mainMenuData.balls[1], "res/balls/volleyball.png");
	ballReset(gameData.renderer, mainMenuData.balls[2], "res/balls/tennisball.png");
	ballReset(gameData.renderer, mainMenuData.balls[3], "res/balls/basketball.png");

	const char* currentlySelectedFilepath = getSettingsValue(gameData.settings, "BALL_TEXTURE_PATH");

	// Sets the current ball selected based on the settings value
	for (int i = 0; i < mainMenuData.balls.size(); i++)
	{
		if (strcmp(currentlySelectedFilepath, mainMenuData.balls[i].texture.filepath) == 0)
		{
			mainMenuData.ballCurrentlySelectedIndex = i;
			mainMenuData.ballInViewIndex = i;
			break;
		}
	}

	updateSelectedBall(gameData);

	gameData.mainMenuInitialised = true;
}

// Called each time entering the state
void mainMenuOnEnter(GameData& gameData)
{
}

MenuButtonSelected menuHandleEvents(GameData& gameData)
{
	if (!gameData.mainMenuInitialised)
	{
		printf("Menu not initialised, cannot handle events.\n");
		return MenuButtonSelected::Exit;
	}
	
	MainMenuData& mainMenuData = gameData.mainMenuData;
	
	switch (mainMenuData.state)
	{
		case MainMenuState::Home:
		{
			switch(gameData.event.type)
			{
				case SDL_MOUSEMOTION:
				{
					menuHandleMouseMove(gameData, mainMenuData.homePageMenu);
				} break;

				case SDL_MOUSEBUTTONDOWN:
				{
					int buttonPressed = menuHandlePress(mainMenuData.homePageMenu);

					if (buttonPressed != -1)
					{
						return (MenuButtonSelected) (buttonPressed + 1);
					}
				} break;

				case SDL_KEYDOWN:
				{
					switch (gameData.event.key.keysym.sym)
					{
						case SDLK_RETURN:
						{
							int buttonPressed = menuHandlePress(mainMenuData.homePageMenu);

							if (buttonPressed != -1)
							{
								return (MenuButtonSelected) (buttonPressed + 1);
							}
						} break;

						case SDLK_RIGHT:
						{
							menuHandleKeyDown(gameData, mainMenuData.homePageMenu);
						} break;

						case SDLK_LEFT:
						{
							menuHandleKeyDown(gameData, mainMenuData.homePageMenu);
						} break;
					}
				} break;
			}
		} break;
	
		case MainMenuState::Customise:
		{
			switch(gameData.event.type)
			{
				case SDL_MOUSEMOTION:
				{
					SDL_Point mousePos = { gameData.event.motion.x, gameData.event.motion.y };

					// Back button
					if (!mainMenuData.backButtonSelected && SDL_PointInRect(&mousePos, &mainMenuData.backButton.rect))
					{
						mainMenuData.backButtonSelected = true;
						mainMenuData.backButton.colour = MENU_COLOURS[1];
						updateTextTexture(gameData.renderer, DIGITAL_DISCO_FONT_PATH, mainMenuData.backButton);
					}
					
					if (mainMenuData.backButtonSelected && !SDL_PointInRect(&mousePos, &mainMenuData.backButton.rect))
					{
						mainMenuData.backButtonSelected = false;
						mainMenuData.backButton.colour = MENU_COLOURS[0];
						updateTextTexture(gameData.renderer, DIGITAL_DISCO_FONT_PATH, mainMenuData.backButton);
					}

					// Highlighting for the arrows
					updateArrowHighlighting(gameData.renderer, mousePos, mainMenuData.ballLeftArrow, mainMenuData.ballLeftArrowSelected);
					updateArrowHighlighting(gameData.renderer, mousePos, mainMenuData.ballRightArrow, mainMenuData.ballRightArrowSelected);
				} break;
				
				case SDL_MOUSEBUTTONDOWN:
				{
					SDL_Point mousePos = { gameData.event.button.x, gameData.event.button.y };
					
					// Back button
					if (mainMenuData.backButtonSelected)
					{
						mainMenuData.state = MainMenuState::Home;

						// Resets view for next entry into the customise state
						mainMenuData.ballInViewIndex = mainMenuData.ballCurrentlySelectedIndex;
						updateSelectedBall(gameData);
						mainMenuData.ballLeftArrowSelected = false;
						mainMenuData.ballRightArrowSelected = false;
						
						break;
					}
					
					if (mainMenuData.ballLeftArrowSelected)
					{
						if (mainMenuData.ballInViewIndex > 0)
						{
							mainMenuData.ballInViewIndex -= 1;
							updateSelectedBall(gameData);
						}
					}
					else if (mainMenuData.ballRightArrowSelected)
					{
						if (mainMenuData.ballInViewIndex < mainMenuData.balls.size() - 1)
						{
							mainMenuData.ballInViewIndex += 1;
							updateSelectedBall(gameData);
						}
					}

					// Main ball in view was clicked
					if (&mainMenuData.balls[mainMenuData.ballInViewIndex].visible && 
						SDL_PointInRect(&mousePos, &mainMenuData.balls[mainMenuData.ballInViewIndex].texture.rect))
					{
						mainMenuData.ballCurrentlySelectedIndex = mainMenuData.ballInViewIndex;
						setSettingsValue(gameData.settings, "BALL_TEXTURE_PATH", mainMenuData.balls[mainMenuData.ballCurrentlySelectedIndex].texture.filepath);
					}
				} break;
			}
		} break;
	}

	// TODO(fkp): Should this be only for Home page
	return MenuButtonSelected::None;
}

void mainMenuDraw(GameData& gameData)
{
	if (!gameData.mainMenuInitialised)
	{
		printf("Menu not initialised, cannot draw.\n");
		return;
	}
	
	MainMenuData& mainMenuData = gameData.mainMenuData;
	
	SDL_SetRenderDrawColor(gameData.renderer, 0, 0, 0, 255);
	SDL_RenderClear(gameData.renderer);

	switch (mainMenuData.state)
	{
		case MainMenuState::Home:
		{
			drawText(gameData.renderer, mainMenuData.title);
			menuDraw(gameData.renderer, mainMenuData.homePageMenu);
		} break;

		case MainMenuState::Customise:
		{
			drawText(gameData.renderer, gameData.mainMenuData.backButton);
			
			drawText(gameData.renderer, gameData.mainMenuData.ballsText);
			drawTexture(gameData.renderer, mainMenuData.ballLeftArrow);
			drawTexture(gameData.renderer, mainMenuData.ballRightArrow, 0.0, SDL_FLIP_HORIZONTAL);
			
			for (int i = 0; i < mainMenuData.balls.size(); i++)
			{
				Ball& ball = mainMenuData.balls[i];
				
				if (ball.visible)
				{
					// Should be highlighted
					if (i == mainMenuData.ballCurrentlySelectedIndex)
					{
						int borderSize;

						if (i == mainMenuData.ballInViewIndex)
						{
							borderSize = 8;
						}
						else
						{
							borderSize = 5;
						}
						mainMenuData.circleHighlight.rect.x = ball.texture.rect.x - borderSize;
						mainMenuData.circleHighlight.rect.y = ball.texture.rect.y - borderSize;
						mainMenuData.circleHighlight.rect.w = ball.texture.rect.w + (borderSize * 2);
						mainMenuData.circleHighlight.rect.h = ball.texture.rect.h + (borderSize * 2);

						drawTexture(gameData.renderer, mainMenuData.circleHighlight);
					}
					
					drawTexture(gameData.renderer, ball.texture);
				}
			}
		} break;
	}

	drawText(gameData.renderer, gameData.fpsText);

	SDL_RenderPresent(gameData.renderer);
}

void updateSelectedBall(GameData& gameData)
{
	std::array<Ball, MENU_CUSTOMISE_NUMBER_OF_BALLS>& balls = gameData.mainMenuData.balls;
	
	for (int i = 0; i < balls.size(); i++)
	{
		// The previous ball
		if (i == gameData.mainMenuData.ballInViewIndex - 1)
		{
			balls[i].visible = true;

			balls[i].texture.rect.w = balls[i].texture.rect.h = MENU_CUSTOMISE_BALL_NOT_IN_VIEW_WIDTH;
			balls[i].texture.rect.x = MENU_CUSTOMISE_BALL_PREV_CENTER_X - (balls[i].texture.rect.w / 2);
			balls[i].texture.rect.y = MENU_CUSTOMISE_BALL_PREV_CENTER_Y - (balls[i].texture.rect.h / 2);
		}
		// The selected ball
		else if (i == gameData.mainMenuData.ballInViewIndex)
		{
			balls[i].visible = true;

			balls[i].texture.rect.w = balls[i].texture.rect.h = MENU_CUSTOMISE_BALL_IN_VIEW_WIDTH;
			balls[i].texture.rect.x = MENU_CUSTOMISE_BALL_IN_VIEW_CENTER_X - (balls[i].texture.rect.w / 2);
			balls[i].texture.rect.y = MENU_CUSTOMISE_BALL_IN_VIEW_CENTER_Y - (balls[i].texture.rect.h / 2);
		}
		// The next ball
		else if (i == gameData.mainMenuData.ballInViewIndex + 1)
		{
			balls[i].visible = true;

			balls[i].texture.rect.w = balls[i].texture.rect.h = MENU_CUSTOMISE_BALL_NOT_IN_VIEW_WIDTH;
			balls[i].texture.rect.x = MENU_CUSTOMISE_BALL_NEXT_CENTER_X - (balls[i].texture.rect.w / 2);
			balls[i].texture.rect.y = MENU_CUSTOMISE_BALL_NEXT_CENTER_Y - (balls[i].texture.rect.h / 2);
		}
		else
		{
			balls[i].visible = false;
		}
	}
}

void updateArrowHighlighting(SDL_Renderer* renderer, const SDL_Point& mousePos, Texture& arrow, bool& arrowSelected)
{
	if (SDL_PointInRect(&mousePos, &arrow.rect))
	{
		if (!arrowSelected)
		{
			arrowSelected = true;
			destroyTexture(arrow);

			int oldX = arrow.rect.x;
			int oldY = arrow.rect.y;
			arrowReset(renderer, arrow, ARROW_HIGHLIGHT_TEXTURE_PATH, oldX, oldY);
		}
	}
	else
	{
		if (arrowSelected)
		{
			arrowSelected = false;
			destroyTexture(arrow);

			int oldX = arrow.rect.x;
			int oldY = arrow.rect.y;
			arrowReset(renderer, arrow, ARROW_TEXTURE_PATH, oldX, oldY);
		}
	}
}
