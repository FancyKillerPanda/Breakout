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
        gameData.menus[a].text = labels[a];
		gameData.menus[a].size = 30;
		gameData.menus[a].colour = MENU_COLOURS[0];
		updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, gameData.menus[a]);
        gameData.menus[a].rect.x = position[a][0] - gameData.menus[a].rect.w / 2;
        gameData.menus[a].rect.y = position[a][1] - gameData.menus[a].rect.h / 2;
    }
}

MenuButtonSelected menuHandleEvents(GameData& gameData)
{
    switch(gameData.event.type)
    {
        case SDL_MOUSEMOTION:
        {
            SDL_Point mousePos = { gameData.event.motion.x, gameData.event.motion.y };
            
            for (int a = 0; a < NUM_ITEMS_IN_MENU; a++)
            {
                if (SDL_PointInRect(&mousePos, &gameData.menus[a].rect))
                {
                    if(!gameData.selected[a])
                    {
                        gameData.selected[a] = 1;
                        gameData.menus[a].colour = MENU_COLOURS[1];
		                updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, gameData.menus[a]);
                    }
                }
                else
                {
                    if(gameData.selected[a])
                    {
                        gameData.selected[a] = 0;
                        gameData.menus[a].colour = MENU_COLOURS[0];
		                updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, gameData.menus[a]);
                    }
                }
            }
        } break;

        case SDL_MOUSEBUTTONDOWN:
        {
            for (int a = 0; a < NUM_ITEMS_IN_MENU; a++)
            {
                if(gameData.selected[a])
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

    return MenuButtonSelected::None;
}

void menuDraw(GameData& gameData)
{
    SDL_SetRenderDrawColor(gameData.renderer, 0, 0, 0, 255);
    SDL_RenderClear(gameData.renderer);

    for (int a = 0; a < NUM_ITEMS_IN_MENU; a++)
    {
        drawText(gameData.renderer, gameData.menus[a]);
    }

    SDL_RenderPresent(gameData.renderer);
}
