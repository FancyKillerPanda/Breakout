#include <SDL/SDL.h>

#include "state.h"		

void gameOverInit(GameData& gameData)
{
    if (gameData.gameOverInitialised)
    {
        return;
    }
    
    GameOverData& gameOverData = gameData.gameOverData;
    
    // Game over text init
    gameOverData.gameOverText.text = "Game Over";
    gameOverData.gameOverText.size = 64;
    updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, gameOverData.gameOverText);
    gameOverData.gameOverText.rect.x = SCREEN_WIDTH / 2 - gameOverData.gameOverText.rect.w / 2;
    gameOverData.gameOverText.rect.y = SCREEN_HEIGHT * 3 / 10 - gameOverData.gameOverText.rect.h / 2;

    // Game over menu items init
    char* labels[NUM_ITEMS_IN_GAME_OVER_MENU] = { "Restart", "Settings", "Exit" };
    int position[NUM_ITEMS_IN_GAME_OVER_MENU][2] = {
        { SCREEN_WIDTH / 4,     SCREEN_HEIGHT / 8 * 7 },
        { SCREEN_WIDTH / 4 * 2, SCREEN_HEIGHT / 8 * 7 }, 
        { SCREEN_WIDTH / 4 * 3, SCREEN_HEIGHT / 8 * 7 }
    };

    for (int a = 0; a < NUM_ITEMS_IN_GAME_OVER_MENU; a++)
    {
        Text& currentItem = gameOverData.menuItems[a];
        
        currentItem.text = labels[a];
        currentItem.size = 30;
        currentItem.colour = MENU_COLOURS[0];
        updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, currentItem);
        currentItem.rect.x = position[a][0] - currentItem.rect.w / 2;
        currentItem.rect.y = position[a][1] - currentItem.rect.h / 2;
    }

    gameOverData.menuItems[0].colour = MENU_COLOURS[1];
    updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, gameOverData.menuItems[0]);
    gameOverData.menuItemSelected[0] = 1;

    gameData.gameOverInitialised = true;
}

bool gameOverHandleEvents(GameData& gameData)
{
    if (!gameData.gameOverInitialised)
    {
        printf("Game over not initialised, cannot handle events.\n");
        return false;
    }

    GameOverData& gameOverData = gameData.gameOverData;

    // TODO(fkp): Extract into functions
    switch (gameData.event.type)
    {
        case SDL_MOUSEMOTION:
        {
            SDL_Point mousePos = { gameData.event.motion.x, gameData.event.motion.y };

            // Removes old selection(s)
            for (int i = 0; i < NUM_ITEMS_IN_MAIN_MENU; i++)
            {
                if (gameOverData.menuItemSelected[i] && !SDL_PointInRect(&mousePos, &gameOverData.menuItems[i].rect))
                {
                    gameOverData.menuItemSelected[i] = 0;
                    gameOverData.menuItems[i].colour = MENU_COLOURS[0];
                    updateTextTexture(gameData.renderer, gameOverData.menuItems[i]);
                }
            }

            for (int a = 0; a < NUM_ITEMS_IN_MAIN_MENU; a++)
            {
                if(!gameOverData.menuItemSelected[a] && SDL_PointInRect(&mousePos, &gameOverData.menuItems[a].rect))
                {
                    // Highlights new selection
                    gameOverData.menuItemSelected[a] = 1;
                    gameOverData.menuItems[a].colour = MENU_COLOURS[1];
                    updateTextTexture(gameData.renderer, gameOverData.menuItems[a]);

                    break;
                }
            }
        } break;

        case SDL_MOUSEBUTTONDOWN:
        {
            for (int a = 0; a < NUM_ITEMS_IN_MAIN_MENU; a++)
            {
                if (gameOverData.menuItemSelected[a])
                {
                    // TODO(fkp): Make this less connected to the order of the options
                    switch (a)
                    {
                        case 0:
                        {
                            changeState(gameData, GameState::Gameplay);
                        } break;

                        case 1:
                        {
                            // TODO(fkp): Make back button come back here
                            changeState(gameData, GameState::MainMenu);
                            gameData.mainMenuData.state = MainMenuState::Customise;
                        } break;

                        case 2:
                        {
                            return false;
                        } break;
                    }
                }
            }
        } break;

        case SDL_KEYDOWN:
        {
            switch (gameData.event.key.keysym.sym)
            {
                case SDLK_RETURN:
                {
                    for (int a = 0; a < NUM_ITEMS_IN_MAIN_MENU; a++)
                    {
                        if(gameOverData.menuItemSelected[a])
                        {
                            // TODO(fkp): Make this less connected to the order of the options
                            switch (a)
                            {
                                case 0:
                                {
                                    changeState(gameData, GameState::Gameplay);
                                } break;

                                case 1:
                                {
                                    changeState(gameData, GameState::MainMenu);
                                    gameData.mainMenuData.state = MainMenuState::Customise;
                                } break;

                                case 2:
                                {
                                    return false;
                                } break;
                            }
                        }
                    }
                } break;

                case SDLK_RIGHT:
                {
                    // Whether an item is already selected
                    bool hit = false;
                            
                    for (int a = 0; a < NUM_ITEMS_IN_MAIN_MENU; a++)
                    {
                        if (gameOverData.menuItemSelected[a])
                        {
                            gameOverData.menuItemSelected[a] = 0;
                            gameOverData.menuItems[a].colour = MENU_COLOURS[0];
                            updateTextTexture(gameData.renderer, gameOverData.menuItems[a]);

                            if (a + 1 >= NUM_ITEMS_IN_MAIN_MENU)
                            {
                                a = -1;
                            }

                            gameOverData.menuItemSelected[a + 1] = 1;
                            gameOverData.menuItems[a + 1].colour = MENU_COLOURS[1];
                            updateTextTexture(gameData.renderer, gameOverData.menuItems[a + 1]);

                            hit = true;
                            break;
                        }
                    }

                    if (!hit)
                    {
                        // Highlights the first option
                        gameOverData.menuItemSelected[0] = 1;
                        gameOverData.menuItems[0].colour = MENU_COLOURS[1];
                        updateTextTexture(gameData.renderer, gameOverData.menuItems[0]);
                    }
                } break;

                case SDLK_LEFT:
                {
                    // Whether an item is already selected
                    bool hit = false;
                            
                    for (int a = 0; a < NUM_ITEMS_IN_MAIN_MENU; a++)
                    {
                        if (gameOverData.menuItemSelected[a])
                        {
                            gameOverData.menuItemSelected[a] = 0;
                            gameOverData.menuItems[a].colour = MENU_COLOURS[0];
                            updateTextTexture(gameData.renderer, gameOverData.menuItems[a]);

                            if (a - 1 < 0)
                            {
                                a = NUM_ITEMS_IN_MAIN_MENU;
                            }

                            gameOverData.menuItemSelected[a - 1] = 1;
                            gameOverData.menuItems[a - 1].colour = MENU_COLOURS[1];
                            updateTextTexture(gameData.renderer, gameOverData.menuItems[a - 1]);

                            hit = true;
                            break;
                        }
                    }

                    if (!hit)
                    {
                        // Highlights the first option
                        gameOverData.menuItemSelected[0] = 1;
                        gameOverData.menuItems[0].colour = MENU_COLOURS[1];
                        updateTextTexture(gameData.renderer, gameOverData.menuItems[0]);
                    }
                } break;
            }
        } break;
    }

    return true;
}

void gameOverDraw(GameData& gameData)
{
    if (!gameData.gameOverInitialised)
    {
        printf("Game over not initialised, cannot draw.\n");
        return;
    }

    GameOverData& gameOverData = gameData.gameOverData;

	SDL_SetRenderDrawColor(gameData.renderer, 0, 0, 0, 255);
	SDL_RenderClear(gameData.renderer);

	SDL_RenderCopy(gameData.renderer, gameData.gameOverData.gameOverText.texture, nullptr, &gameData.gameOverData.gameOverText.rect);

    for (int a = 0; a < NUM_ITEMS_IN_GAME_OVER_MENU; a++)
    {
        drawText(gameData.renderer, gameOverData.menuItems[a]);
    }

	SDL_RenderPresent(gameData.renderer);
}
