#include <SDL/SDL.h>

#include "state.h"

void initMenu(GameData& gameData)
{
    MenuData& menuData = gameData.menuData;
    
    // Home page init
    char* labels[NUM_ITEMS_IN_MENU] = { "Start", "Settings", "Exit" };
    int position[NUM_ITEMS_IN_MENU][2] = {
        { SCREEN_WIDTH / 4,     SCREEN_HEIGHT / 8 * 7 },
        { SCREEN_WIDTH / 4 * 2, SCREEN_HEIGHT / 8 * 7 }, 
        { SCREEN_WIDTH / 4 * 3, SCREEN_HEIGHT / 8 * 7 }
    };

    for (int a = 0; a < NUM_ITEMS_IN_MENU; a++)
    {
        Text& currentItem = menuData.homeMenuItems[a];
        
        currentItem.text = labels[a];
        currentItem.size = 30;
        currentItem.colour = MENU_COLOURS[0];
		updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, currentItem);
        currentItem.rect.x = position[a][0] - currentItem.rect.w / 2;
        currentItem.rect.y = position[a][1] - currentItem.rect.h / 2;
    }

    // Customise page init
    menuData.ballsText.colour = MENU_COLOURS[0];
    menuData.ballsText.text = "Ball:"; // TODO(fkp): Font does not render colon on the end
    menuData.ballsText.size = 30; // TODO(fkp): Extract font size into constant
	updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, menuData.ballsText);
    menuData.ballsText.rect.x = MENU_CUSTOMISE_BALL_TEXT_X;
    menuData.ballsText.rect.y = MENU_CUSTOMISE_BALL_TEXT_CENTER_Y - menuData.ballsText.rect.h / 2;
    
    menuData.ballLeftArrow = createTexture(gameData.renderer, "res/arrow.png");
    menuData.ballLeftArrow.rect.w = MENU_CUSTOMISE_BALL_NOT_IN_VIEW_WIDTH / 2;
    menuData.ballLeftArrow.rect.h = MENU_CUSTOMISE_BALL_NOT_IN_VIEW_WIDTH;
    menuData.ballLeftArrow.rect.x = MENU_CUSTOMISE_BALL_PREV_CENTER_X - (MENU_CUSTOMISE_BALL_NOT_IN_VIEW_WIDTH / 2) - 30 - menuData.ballLeftArrow.rect.w;
    menuData.ballLeftArrow.rect.y = MENU_CUSTOMISE_BALL_PREV_CENTER_Y - menuData.ballLeftArrow.rect.h / 2;
    
    menuData.ballRightArrow = createTexture(gameData.renderer, "res/arrow.png");
    menuData.ballRightArrow.rect.w = MENU_CUSTOMISE_BALL_NOT_IN_VIEW_WIDTH / 2;
    menuData.ballRightArrow.rect.h = MENU_CUSTOMISE_BALL_NOT_IN_VIEW_WIDTH;
    menuData.ballRightArrow.rect.x = MENU_CUSTOMISE_BALL_NEXT_CENTER_X + (MENU_CUSTOMISE_BALL_NOT_IN_VIEW_WIDTH / 2) + 30;
    menuData.ballRightArrow.rect.y = MENU_CUSTOMISE_BALL_NEXT_CENTER_Y - menuData.ballRightArrow.rect.h / 2;
    
    ballReset(gameData.renderer, menuData.balls[0], "res/balls/default_ball.png");
    ballReset(gameData.renderer, menuData.balls[1], "res/balls/volleyball.png");
    ballReset(gameData.renderer, menuData.balls[2], "res/balls/basketball.png");

    updateSelectedBall(gameData);
}

MenuButtonSelected menuHandleEvents(GameData& gameData)
{
    MenuData& menuData = gameData.menuData;
    
    switch (menuData.state)
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
                        if (SDL_PointInRect(&mousePos, &menuData.homeMenuItems[a].rect))
                        {
                            if(!menuData.homeMenuItemSelected[a])
                            {
                                menuData.homeMenuItemSelected[a] = 1;
                                menuData.homeMenuItems[a].colour = MENU_COLOURS[1];
                                updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, menuData.homeMenuItems[a]);
                            }
                        }
                        else
                        {
                            if(menuData.homeMenuItemSelected[a])
                            {
                                menuData.homeMenuItemSelected[a] = 0;
                                menuData.homeMenuItems[a].colour = MENU_COLOURS[0];
                                updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, menuData.homeMenuItems[a]);
                            }
                        }
                    }
                } break;

                case SDL_MOUSEBUTTONDOWN:
                {
                    for (int a = 0; a < NUM_ITEMS_IN_MENU; a++)
                    {
                        if(menuData.homeMenuItemSelected[a])
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
                        if (menuData.ballSelectedIndex < menuData.balls.size() - 1)
                        {
                            menuData.ballSelectedIndex += 1;
                            updateSelectedBall(gameData);
                        }
                    }
                    else if (gameData.event.key.keysym.sym == SDLK_LEFT)
                    {
                        if (menuData.ballSelectedIndex > 0)
                        {
                            menuData.ballSelectedIndex -= 1;
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
    MenuData& menuData = gameData.menuData;
    
    SDL_SetRenderDrawColor(gameData.renderer, 0, 0, 0, 255);
    SDL_RenderClear(gameData.renderer);

    switch (menuData.state)
    {
        case MenuState::Home:
        {
            for (int a = 0; a < NUM_ITEMS_IN_MENU; a++)
            {
                drawText(gameData.renderer, menuData.homeMenuItems[a]);
            }
        } break;

        case MenuState::Customise:
        {
            drawText(gameData.renderer, gameData.menuData.ballsText);
            drawTexture(gameData.renderer, menuData.ballLeftArrow);
            drawTexture(gameData.renderer, menuData.ballRightArrow, 0.0, SDL_FLIP_HORIZONTAL);
            
            for (Ball& ball : menuData.balls)
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
    std::array<Ball, 3Ui64>& balls = gameData.menuData.balls;
    
    for (int i = 0; i < balls.size(); i++)
    {
        // The previous ball
        if (i == gameData.menuData.ballSelectedIndex - 1)
        {
            balls[i].visible = true;

            balls[i].texture.rect.w = balls[i].texture.rect.h = MENU_CUSTOMISE_BALL_NOT_IN_VIEW_WIDTH;
            balls[i].texture.rect.x = MENU_CUSTOMISE_BALL_PREV_CENTER_X - (balls[i].texture.rect.w / 2);
            balls[i].texture.rect.y = MENU_CUSTOMISE_BALL_PREV_CENTER_Y - (balls[i].texture.rect.h / 2);
        }
        // The selected ball
        else if (i == gameData.menuData.ballSelectedIndex)
        {
            balls[i].visible = true;

            balls[i].texture.rect.w = balls[i].texture.rect.h = MENU_CUSTOMISE_BALL_IN_VIEW_WIDTH;
            balls[i].texture.rect.x = MENU_CUSTOMISE_BALL_IN_VIEW_CENTER_X - (balls[i].texture.rect.w / 2);
            balls[i].texture.rect.y = MENU_CUSTOMISE_BALL_IN_VIEW_CENTER_Y - (balls[i].texture.rect.h / 2);
        }
        // The next ball
        else if (i == gameData.menuData.ballSelectedIndex + 1)
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
