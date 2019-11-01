#include <SDL/SDL.h>

#include "state.h"

void initMenu(GameData& gameData)
{
    char* labels[NUM_ITEMS_IN_MENU] = {"Start", "Settings", "Exit"};
    int position[NUM_ITEMS_IN_MENU][2] = {{SCREEN_WIDTH/4, SCREEN_HEIGHT/8*7}, 
                                {SCREEN_WIDTH/4*2, SCREEN_HEIGHT/8*7}, 
                                {SCREEN_WIDTH/4*3, SCREEN_HEIGHT/8*7}};

    for (int a = 0; a < NUM_ITEMS_IN_MENU; a++)
    {
        gameData.menus[a].text = labels[a];
		gameData.menus[a].size = 30;
		gameData.menus[a].colour = MENU_COLOURS[0];
		updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, gameData.menus[a]);
        gameData.menus[a].rect.x = position[a][0] - gameData.menus[a].rect.w/2;
        gameData.menus[a].rect.y = position[a][1] - gameData.menus[a].rect.h/2;
    }
    gameData.menus[0].colour = MENU_COLOURS[1];
    updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, gameData.menus[0]);
    gameData.selected[0] = 1;
}

MenuButtonSelected menuHandleEvents(GameData& gameData)
{
    int x, y;
    switch(gameData.event.type)
    {
        case SDL_MOUSEMOTION:
        {
            x = gameData.event.motion.x;
            y = gameData.event.motion.y;
            for (int a = 0; a < NUM_ITEMS_IN_MENU; a++)
            {
                if(x >= gameData.menus[a].rect.x && x <= gameData.menus[a].rect.x + gameData.menus[a].rect.w && y >= gameData.menus[a].rect.y && y <= gameData.menus[a].rect.y + gameData.menus[a].rect.h)
                {
                    for (int b = 0; b < NUM_ITEMS_IN_MENU; b++)
                    {
                        if (b == a)
                        {
                            gameData.selected[b] = 1;
                            gameData.menus[b].colour = MENU_COLOURS[1];
                            updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, gameData.menus[b]);
                        }
                        else
                        {
                            gameData.selected[b] = 0;
                            gameData.menus[b].colour = MENU_COLOURS[0];
                            updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, gameData.menus[b]);
                        }
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
        }

        case SDL_KEYDOWN:
        {
            switch (gameData.event.key.keysym.sym)
            {
                case SDLK_RETURN:
                {
                    for (int a = 0; a < NUM_ITEMS_IN_MENU; a++)
                    {
                        if (gameData.selected[a] == 1)
                        {
                            return (MenuButtonSelected) (a + 1);
                        }
                    }
                } break;

                case SDLK_RIGHT:
                {
                    for (int a = 0; a < NUM_ITEMS_IN_MENU; a++)
                    {
                        if (gameData.selected[a] == 1)
                        {
                            gameData.selected[a] = 0;
                            gameData.menus[a].colour = MENU_COLOURS[0];
                            updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, gameData.menus[a]);

                            if (a + 1 >= NUM_ITEMS_IN_MENU)
                            {
                                a = -1;
                            }

                            gameData.selected[a + 1] = 1;
                            gameData.menus[a + 1].colour = MENU_COLOURS[1];
                            updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, gameData.menus[a + 1]);
                            break;
                        }
                    }
                } break;

                case SDLK_LEFT:
                {
                    for (int a = 0; a < NUM_ITEMS_IN_MENU; a++)
                    {
                        if (gameData.selected[a] == 1)
                        {
                            gameData.selected[a] = 0;
                            gameData.menus[a].colour = MENU_COLOURS[0];
                            updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, gameData.menus[a]);

                            if (a - 1 < 0)
                            {
                                a = NUM_ITEMS_IN_MENU;
                            }

                            gameData.selected[a - 1] = 1;
                            gameData.menus[a - 1].colour = MENU_COLOURS[1];
                            updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, gameData.menus[a - 1]);
                            break;
                        }
                    }
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
