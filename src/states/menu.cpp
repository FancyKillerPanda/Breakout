#include <SDL/SDL.h>

#include "state.h"

void initMenu(GameData& gameData)
{
    char* labels[NUM_ITEMS_IN_MENU] = { "Start", "Settings", "Exit" };
    int position[NUM_ITEMS_IN_MENU][2] = {
        { SCREEN_WIDTH / 4,     SCREEN_HEIGHT / 8 * 7 },
        { SCREEN_WIDTH / 4 * 2, SCREEN_HEIGHT / 8 * 7 }, 
        { SCREEN_WIDTH / 4 * 3, SCREEN_HEIGHT / 8 * 7 }
    };

    for (int a = 0; a < NUM_ITEMS_IN_MENU; a++)
    {
        gameData.menuData.homeMenuItems[a].text = labels[a];
		gameData.menuData.homeMenuItems[a].size = 30;
		gameData.menuData.homeMenuItems[a].colour = MENU_COLOURS[0];
		updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, gameData.menuData.homeMenuItems[a]);
        gameData.menuData.homeMenuItems[a].rect.x = position[a][0] - gameData.menuData.homeMenuItems[a].rect.w / 2;
        gameData.menuData.homeMenuItems[a].rect.y = position[a][1] - gameData.menuData.homeMenuItems[a].rect.h / 2;
    }
}

MenuButtonSelected menuHandleEvents(GameData& gameData)
{
    switch (gameData.menuData.state)
    {
        case MenuState::Home:
        {
            switch(gameData.event.type)
            {
                case SDL_MOUSEMOTION:
                {
                    SDL_Point mousePos = { gameData.event.motion.x, gameData.event.motion.y };
                    
                    for (int a = 0; a < NUM_ITEMS_IN_MENU; a++)
                    {
                        if (SDL_PointInRect(&mousePos, &gameData.menuData.homeMenuItems[a].rect))
                        {
                            if(!gameData.menuData.homeMenuItemSelected[a])
                            {
                                gameData.menuData.homeMenuItemSelected[a] = 1;
                                gameData.menuData.homeMenuItems[a].colour = MENU_COLOURS[1];
                                updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, gameData.menuData.homeMenuItems[a]);
                            }
                        }
                        else
                        {
                            if(gameData.menuData.homeMenuItemSelected[a])
                            {
                                gameData.menuData.homeMenuItemSelected[a] = 0;
                                gameData.menuData.homeMenuItems[a].colour = MENU_COLOURS[0];
                                updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, gameData.menuData.homeMenuItems[a]);
                            }
                        }
                    }
                } break;

                case SDL_MOUSEBUTTONDOWN:
                {
                    for (int a = 0; a < NUM_ITEMS_IN_MENU; a++)
                    {
                        if(gameData.menuData.homeMenuItemSelected[a])
                        {
                            return (MenuButtonSelected) (a + 1);
                        }
                    }
                } break;

                case SDL_KEYDOWN:
                {
                    switch (gameData.event.key.keysym.sym)
                    {
                        case SDLK_RETURN:
                        {
                            return MenuButtonSelected::Start;
                        } break;
                    }
                } break;
            }
        } break;
    
        case MenuState::Customise:
        {
            // TODO(fkp): Customization event handling
        } break;
    }

    // TODO(fkp): Should this be only for Home page
    return MenuButtonSelected::None;
}

void menuDraw(GameData& gameData)
{
    SDL_SetRenderDrawColor(gameData.renderer, 0, 0, 0, 255);
    SDL_RenderClear(gameData.renderer);

    switch (gameData.menuData.state)
    {
        case MenuState::Home:
        {
            for (int a = 0; a < NUM_ITEMS_IN_MENU; a++)
            {
                drawText(gameData.renderer, gameData.menuData.homeMenuItems[a]);
            }
        } break;

        case MenuState::Customise:
        {
            // TODO(fkp): Draw customization screen
            SDL_SetRenderDrawColor(gameData.renderer, 255, 0, 0, 255);
            SDL_RenderClear(gameData.renderer);
        } break;
    }

    SDL_RenderPresent(gameData.renderer);
}
