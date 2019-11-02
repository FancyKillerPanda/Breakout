#include <SDL/SDL.h>

#include "state.h"

void initMenu(GameData& gameData)
{
    // Home page init
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

    // Customise page init
    ballReset(gameData.renderer, gameData.menuData.balls[0], "res/balls/default_ball.png");
    ballReset(gameData.renderer, gameData.menuData.balls[1], "res/balls/volleyball.png");
    ballReset(gameData.renderer, gameData.menuData.balls[2], "res/balls/basketball.png");

    updateSelectedBall(gameData);
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
            switch(gameData.event.type)
            {
                case SDL_KEYDOWN:
                {
                    // TODO(fkp): Change the keys for this later
                    if (gameData.event.key.keysym.sym == SDLK_RIGHT)
                    {
                        if (gameData.menuData.ballSelectedIndex < gameData.menuData.balls.size() - 1)
                        {
                            gameData.menuData.ballSelectedIndex += 1;
                            updateSelectedBall(gameData);
                        }
                    }
                    else if (gameData.event.key.keysym.sym == SDLK_LEFT)
                    {
                        if (gameData.menuData.ballSelectedIndex > 0)
                        {
                            gameData.menuData.ballSelectedIndex -= 1;
                            updateSelectedBall(gameData);
                        }
                    }
                } break;
            }
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
            for (Ball& ball : gameData.menuData.balls)
            {
                if (ball.visible)
                {
                    drawTexture(gameData.renderer, ball.texture);
                }
            }
        } break;
    }

    SDL_RenderPresent(gameData.renderer);
}

void updateSelectedBall(GameData& gameData)
{
    for (int i = 0; i < gameData.menuData.balls.size(); i++)
    {
        // The previous ball
        if (i == gameData.menuData.ballSelectedIndex - 1)
        {
            gameData.menuData.balls[i].visible = true;

            gameData.menuData.balls[i].texture.rect.w = gameData.menuData.balls[i].texture.rect.h = MENU_CUSTOMISE_BALL_NOT_IN_VIEW_WIDTH;
            gameData.menuData.balls[i].texture.rect.x = MENU_CUSTOMISE_BALL_PREV_CENTER_X - (gameData.menuData.balls[i].texture.rect.w / 2);
            gameData.menuData.balls[i].texture.rect.y = MENU_CUSTOMISE_BALL_PREV_CENTER_Y - (gameData.menuData.balls[i].texture.rect.h / 2);
        }
        // The selected ball
        else if (i == gameData.menuData.ballSelectedIndex)
        {
            gameData.menuData.balls[i].visible = true;

            gameData.menuData.balls[i].texture.rect.w = gameData.menuData.balls[i].texture.rect.h = MENU_CUSTOMISE_BALL_IN_VIEW_WIDTH;
            gameData.menuData.balls[i].texture.rect.x = MENU_CUSTOMISE_BALL_IN_VIEW_CENTER_X - (gameData.menuData.balls[i].texture.rect.w / 2);
            gameData.menuData.balls[i].texture.rect.y = MENU_CUSTOMISE_BALL_IN_VIEW_CENTER_Y - (gameData.menuData.balls[i].texture.rect.h / 2);
        }
        // The next ball
        else if (i == gameData.menuData.ballSelectedIndex + 1)
        {
            gameData.menuData.balls[i].visible = true;

            gameData.menuData.balls[i].texture.rect.w = gameData.menuData.balls[i].texture.rect.h = MENU_CUSTOMISE_BALL_NOT_IN_VIEW_WIDTH;
            gameData.menuData.balls[i].texture.rect.x = MENU_CUSTOMISE_BALL_NEXT_CENTER_X - (gameData.menuData.balls[i].texture.rect.w / 2);
            gameData.menuData.balls[i].texture.rect.y = MENU_CUSTOMISE_BALL_NEXT_CENTER_Y - (gameData.menuData.balls[i].texture.rect.h / 2);
        }
        else
        {
            gameData.menuData.balls[i].visible = false;
        }
    }
}
