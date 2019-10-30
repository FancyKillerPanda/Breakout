#include <SDL/SDL.h>

#include "state.h"

int initMenu(GameData& gameData)
{
    return 0;
}

bool menuUpdate(GameData &gameData)
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
		                updateTextTexture(gameData.renderer, ARIAL_FONT_PATH, gameData.menus[a]);
                    }
                }
                else
                {
                    if(gameData.selected[a])
                    {
                        gameData.selected[a] = 0;
                        gameData.menus[a].colour = MENUCOLOURS[0];
		                updateTextTexture(gameData.renderer, ARIAL_FONT_PATH, gameData.menus[a]);
                    }
                }
            }
        } break;
    } 
    return true;
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