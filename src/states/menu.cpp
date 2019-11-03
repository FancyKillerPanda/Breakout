#include <SDL/SDL.h>

#include "state.h"

void initMenu(GameData& gameData)
{
    MenuData& menuData = gameData.menuData;
    
    // Highlighting texture
    menuData.circleHighlight = createTexture(gameData.renderer, "res/circle_highlight.png");
    
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
    
    arrowReset(gameData.renderer, menuData.ballLeftArrow, ARROW_TEXTURE_PATH);
    menuData.ballLeftArrow.rect.x = MENU_CUSTOMISE_BALL_PREV_CENTER_X - (MENU_CUSTOMISE_BALL_NOT_IN_VIEW_WIDTH / 2) - 30 - menuData.ballLeftArrow.rect.w;
    menuData.ballLeftArrow.rect.y = MENU_CUSTOMISE_BALL_PREV_CENTER_Y - menuData.ballLeftArrow.rect.h / 2;

    arrowReset(gameData.renderer, menuData.ballRightArrow, ARROW_TEXTURE_PATH);
    menuData.ballRightArrow.rect.x = MENU_CUSTOMISE_BALL_NEXT_CENTER_X + (MENU_CUSTOMISE_BALL_NOT_IN_VIEW_WIDTH / 2) + 30;
    menuData.ballRightArrow.rect.y = MENU_CUSTOMISE_BALL_NEXT_CENTER_Y - menuData.ballRightArrow.rect.h / 2;
    
    ballReset(gameData.renderer, menuData.balls[0], "res/balls/default_ball.png");
    ballReset(gameData.renderer, menuData.balls[1], "res/balls/volleyball.png");
    ballReset(gameData.renderer, menuData.balls[2], "res/balls/tennisball.png");
    ballReset(gameData.renderer, menuData.balls[3], "res/balls/basketball.png");

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
                case SDL_MOUSEMOTION:
                {
                    SDL_Point mousePos = { gameData.event.motion.x, gameData.event.motion.y };

                    // TODO(fkp): Extract into a function
                    // Highlighting for the left ball arrow
                    if (SDL_PointInRect(&mousePos, &menuData.ballLeftArrow.rect))
                    {
                        if (!menuData.ballLeftArrowSelected)
                        {
                            menuData.ballLeftArrowSelected = true;
                            destroyTexture(menuData.ballLeftArrow);

                            int oldX = menuData.ballLeftArrow.rect.x;
                            int oldY = menuData.ballLeftArrow.rect.y;
                            arrowReset(gameData.renderer, menuData.ballLeftArrow, ARROW_HIGHLIGHT_TEXTURE_PATH, oldX, oldY);
                        }
                    }
                    else
                    {
                        if (menuData.ballLeftArrowSelected)
                        {
                            menuData.ballLeftArrowSelected = false;
                            destroyTexture(menuData.ballLeftArrow);

                            int oldX = menuData.ballLeftArrow.rect.x;
                            int oldY = menuData.ballLeftArrow.rect.y;
                            arrowReset(gameData.renderer, menuData.ballLeftArrow, ARROW_TEXTURE_PATH, oldX, oldY);
                        }
                    }

                    // Highlighting for the right ball arrow
                    if (SDL_PointInRect(&mousePos, &menuData.ballRightArrow.rect))
                    {
                        if (!menuData.ballRightArrowSelected)
                        {
                            menuData.ballRightArrowSelected = true;
                            destroyTexture(menuData.ballRightArrow);

                            int oldX = menuData.ballRightArrow.rect.x;
                            int oldY = menuData.ballRightArrow.rect.y;
                            arrowReset(gameData.renderer, menuData.ballRightArrow, ARROW_HIGHLIGHT_TEXTURE_PATH, oldX, oldY);

                        }
                    }
                    else
                    {
                        if (menuData.ballRightArrowSelected)
                        {
                            menuData.ballRightArrowSelected = false;
                            destroyTexture(menuData.ballRightArrow);

                            int oldX = menuData.ballRightArrow.rect.x;
                            int oldY = menuData.ballRightArrow.rect.y;
                            arrowReset(gameData.renderer, menuData.ballRightArrow, ARROW_TEXTURE_PATH, oldX, oldY);
                        }
                    }
                } break;
                
                case SDL_MOUSEBUTTONDOWN:
                {
                    SDL_Point mousePos = { gameData.event.button.x, gameData.event.button.y };
                    
                    if (menuData.ballLeftArrowSelected)
                    {
                        if (menuData.ballInViewIndex > 0)
                        {
                            menuData.ballInViewIndex -= 1;
                            updateSelectedBall(gameData);
                        }
                    }
                    else if (menuData.ballRightArrowSelected)
                    {
                        if (menuData.ballInViewIndex < menuData.balls.size() - 1)
                        {
                            menuData.ballInViewIndex += 1;
                            updateSelectedBall(gameData);
                        }
                    }

                    // Main ball in view was clicked
                    if (&menuData.balls[menuData.ballInViewIndex].visible && 
                        SDL_PointInRect(&mousePos, &menuData.balls[menuData.ballInViewIndex].texture.rect))
                    {
                        menuData.ballCurrentlySelectedIndex = menuData.ballInViewIndex;
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
            
            for (int i = 0; i < menuData.balls.size(); i++)
            {
                Ball& ball = menuData.balls[i];
                
                if (ball.visible)
                {
                    // Should be highlighted
                    if (i == menuData.ballCurrentlySelectedIndex)
                    {
                        int borderSize;

                        if (i == menuData.ballInViewIndex)
                        {
                            borderSize = 8;
                        }
                        else
                        {
                            borderSize = 5;
                        }
                        menuData.circleHighlight.rect.x = ball.texture.rect.x - borderSize;
                        menuData.circleHighlight.rect.y = ball.texture.rect.y - borderSize;
                        menuData.circleHighlight.rect.w = ball.texture.rect.w + (borderSize * 2);
                        menuData.circleHighlight.rect.h = ball.texture.rect.h + (borderSize * 2);

                        drawTexture(gameData.renderer, menuData.circleHighlight);
                    }
                    
                    drawTexture(gameData.renderer, ball.texture);
                }
            }
        } break;
    }

    SDL_RenderPresent(gameData.renderer);
}

void updateSelectedBall(GameData& gameData)
{
    std::array<Ball, MENU_CUSTOMISE_NUMBER_OF_BALLS>& balls = gameData.menuData.balls;
    
    for (int i = 0; i < balls.size(); i++)
    {
        // The previous ball
        if (i == gameData.menuData.ballInViewIndex - 1)
        {
            balls[i].visible = true;

            balls[i].texture.rect.w = balls[i].texture.rect.h = MENU_CUSTOMISE_BALL_NOT_IN_VIEW_WIDTH;
            balls[i].texture.rect.x = MENU_CUSTOMISE_BALL_PREV_CENTER_X - (balls[i].texture.rect.w / 2);
            balls[i].texture.rect.y = MENU_CUSTOMISE_BALL_PREV_CENTER_Y - (balls[i].texture.rect.h / 2);
        }
        // The selected ball
        else if (i == gameData.menuData.ballInViewIndex)
        {
            balls[i].visible = true;

            balls[i].texture.rect.w = balls[i].texture.rect.h = MENU_CUSTOMISE_BALL_IN_VIEW_WIDTH;
            balls[i].texture.rect.x = MENU_CUSTOMISE_BALL_IN_VIEW_CENTER_X - (balls[i].texture.rect.w / 2);
            balls[i].texture.rect.y = MENU_CUSTOMISE_BALL_IN_VIEW_CENTER_Y - (balls[i].texture.rect.h / 2);
        }
        // The next ball
        else if (i == gameData.menuData.ballInViewIndex + 1)
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
