#include <SDL/SDL.h>

#include "state.h"

void initMenu(GameData& gameData)
{
    char* labels[NUMMENU] = {"Start", "Settings", "Exit"};
    int position[NUMMENU][2] = {{SCREEN_WIDTH/4, SCREEN_HEIGHT/8*7}, 
                                {SCREEN_WIDTH/4*2, SCREEN_HEIGHT/8*7}, 
                                {SCREEN_WIDTH/4*3, SCREEN_HEIGHT/8*7}};

    for (int a = 0; a < NUMMENU; a++)
    {
        gameData.menus[a].text = labels[a];
		gameData.menus[a].size = 30;
		gameData.menus[a].colour = MENUCOLOURS[0];
		updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, gameData.menus[a]);
        gameData.menus[a].rect.x = position[a][0] - gameData.menus[a].rect.w/2;
        gameData.menus[a].rect.y = position[a][1] - gameData.menus[a].rect.h/2;
    }
}

int menuHandleEvents(GameData& gameData)
{
    int x, y;
    switch(gameData.event.type)
    {
        case SDL_MOUSEMOTION:
        {
            x = gameData.event.motion.x;
            y = gameData.event.motion.y;
            for (int a = 0; a < NUMMENU; a++)
            {
                if(x >= gameData.menus[a].rect.x && x <= gameData.menus[a].rect.x + gameData.menus[a].rect.w && y >= gameData.menus[a].rect.y && y <= gameData.menus[a].rect.y + gameData.menus[a].rect.h)
                {
                    if(!gameData.selected[a])
                    {
                        gameData.selected[a] = 1;
                        gameData.menus[a].colour = MENUCOLOURS[1];
		                updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, gameData.menus[a]);
                    }
                }
                else
                {
                    if(gameData.selected[a])
                    {
                        gameData.selected[a] = 0;
                        gameData.menus[a].colour = MENUCOLOURS[0];
		                updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, gameData.menus[a]);
                    }
                }
            }
        } break;
        case SDL_MOUSEBUTTONDOWN:
        {
            for (int a = 0; a < NUMMENU; a++)
            {
                if(gameData.selected[a])
                {
                    return a + 1;
                }
            }
        }
    } 
    return 0;
}

void menuDraw(GameData& gameData)
{
    SDL_SetRenderDrawColor(gameData.renderer, 0, 0, 0, 255);
    SDL_RenderClear(gameData.renderer);

    for (int a = 0; a < NUMMENU; a++)
    {
        SDL_RenderCopy(gameData.renderer, gameData.menus[a].texture, nullptr, &gameData.menus[a].rect);
    }

    SDL_RenderPresent(gameData.renderer);
}