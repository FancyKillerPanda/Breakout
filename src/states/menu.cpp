#include <SDL/SDL.h>

#include "state.h"

void menuInit(GameData& gameData)
{
    if (gameData.menuInitialised)
    {
        return;
    }

    MenuData& menuData = gameData.menuData;

    // Back button
    menuData.backButton.text = "Back";
    menuData.backButton.size = 24;
    menuData.backButton.colour = MENU_COLOURS[0];
    updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, menuData.backButton);
    menuData.backButton.rect.x = SCREEN_WIDTH / 10;
    menuData.backButton.rect.y = SCREEN_HEIGHT * 7 / 8;
    
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

    menuData.homeMenuItems[0].colour = MENU_COLOURS[1];
    updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, menuData.homeMenuItems[0]);
    menuData.homeMenuItemSelected[0] = 1;

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

    gameData.menuInitialised = true;
}

// Called each time entering the state
void menuOnEnter(GameData& gameData)
{
}

MenuButtonSelected menuHandleEvents(GameData& gameData)
{
    if (!gameData.menuInitialised)
    {
        printf("Menu not initialised, cannot handle events.\n");
        return MenuButtonSelected::Exit;
    }
    
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

                    // Removes old selection(s)
                    for (int i = 0; i < NUM_ITEMS_IN_MENU; i++)
                    {
                        if (menuData.homeMenuItemSelected[i] && !SDL_PointInRect(&mousePos, &menuData.homeMenuItems[i].rect))
                        {
                            menuData.homeMenuItemSelected[i] = 0;
                            menuData.homeMenuItems[i].colour = MENU_COLOURS[0];
                            updateTextTexture(gameData.renderer, menuData.homeMenuItems[i]);
                        }
                    }

                    for (int a = 0; a < NUM_ITEMS_IN_MENU; a++)
                    {
                        if(!menuData.homeMenuItemSelected[a] && SDL_PointInRect(&mousePos, &menuData.homeMenuItems[a].rect))
                        {
                            // Highlights new selection
                            menuData.homeMenuItemSelected[a] = 1;
                            menuData.homeMenuItems[a].colour = MENU_COLOURS[1];
                            updateTextTexture(gameData.renderer, menuData.homeMenuItems[a]);

                            break;
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
                            for (int a = 0; a < NUM_ITEMS_IN_MENU; a++)
                            {
                                if(menuData.homeMenuItemSelected[a])
                                {
                                    return (MenuButtonSelected) (a + 1);
                                }
                            }
                        } break;

                        case SDLK_RIGHT:
                        {
                            // Whether an item is already selected
                            bool hit = false;
                            
                            for (int a = 0; a < NUM_ITEMS_IN_MENU; a++)
                            {
                                if (menuData.homeMenuItemSelected[a])
                                {
                                    menuData.homeMenuItemSelected[a] = 0;
                                    menuData.homeMenuItems[a].colour = MENU_COLOURS[0];
                                    updateTextTexture(gameData.renderer, menuData.homeMenuItems[a]);

                                    if (a + 1 >= NUM_ITEMS_IN_MENU)
                                    {
                                        a = -1;
                                    }

                                    menuData.homeMenuItemSelected[a + 1] = 1;
                                    menuData.homeMenuItems[a + 1].colour = MENU_COLOURS[1];
                                    updateTextTexture(gameData.renderer, menuData.homeMenuItems[a + 1]);

                                    hit = true;
                                    break;
                                }
                            }

                            if (!hit)
                            {
                                // Highlights the first option
                                menuData.homeMenuItemSelected[0] = 1;
                                menuData.homeMenuItems[0].colour = MENU_COLOURS[1];
                                updateTextTexture(gameData.renderer, menuData.homeMenuItems[0]);
                            }
                        } break;

                        case SDLK_LEFT:
                        {
                            // Whether an item is already selected
                            bool hit = false;
                            
                            for (int a = 0; a < NUM_ITEMS_IN_MENU; a++)
                            {
                                if (menuData.homeMenuItemSelected[a])
                                {
                                    menuData.homeMenuItemSelected[a] = 0;
                                    menuData.homeMenuItems[a].colour = MENU_COLOURS[0];
                                    updateTextTexture(gameData.renderer, menuData.homeMenuItems[a]);

                                    if (a - 1 < 0)
                                    {
                                        a = NUM_ITEMS_IN_MENU;
                                    }

                                    menuData.homeMenuItemSelected[a - 1] = 1;
                                    menuData.homeMenuItems[a - 1].colour = MENU_COLOURS[1];
                                    updateTextTexture(gameData.renderer, menuData.homeMenuItems[a - 1]);

                                    hit = true;
                                    break;
                                }
                            }

                            if (!hit)
                            {
                                // Highlights the first option
                                menuData.homeMenuItemSelected[0] = 1;
                                menuData.homeMenuItems[0].colour = MENU_COLOURS[1];
                                updateTextTexture(gameData.renderer, menuData.homeMenuItems[0]);
                            }
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

                    // Back button
                    if (!menuData.backButtonSelected && SDL_PointInRect(&mousePos, &menuData.backButton.rect))
                    {
                        menuData.backButtonSelected = true;
                        menuData.backButton.colour = MENU_COLOURS[1];
                        updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, menuData.backButton);
                    }
                    
                    if (menuData.backButtonSelected && !SDL_PointInRect(&mousePos, &menuData.backButton.rect))
                    {
                        menuData.backButtonSelected = false;
                        menuData.backButton.colour = MENU_COLOURS[0];
                        updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, menuData.backButton);
                    }

                    // Highlighting for the arrows
                    updateArrowHighlighting(gameData.renderer, mousePos, menuData.ballLeftArrow, menuData.ballLeftArrowSelected);
                    updateArrowHighlighting(gameData.renderer, mousePos, menuData.ballRightArrow, menuData.ballRightArrowSelected);
                } break;
                
                case SDL_MOUSEBUTTONDOWN:
                {
                    SDL_Point mousePos = { gameData.event.button.x, gameData.event.button.y };
                    
                    // Back button
                    if (menuData.backButtonSelected)
                    {
                        menuData.state = MenuState::Home;

                        // Resets view for next entry into the customise state
                        menuData.ballInViewIndex = menuData.ballCurrentlySelectedIndex;
                        updateSelectedBall(gameData);
                        menuData.ballLeftArrowSelected = false;
                        menuData.ballRightArrowSelected = false;
                        
                        break;
                    }
                    
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
                        gameData.ballFilepath = menuData.balls[menuData.ballCurrentlySelectedIndex].texture.filepath;
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
    if (!gameData.menuInitialised)
    {
        printf("Menu not initialised, cannot draw.\n");
        return;
    }
    
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
            drawText(gameData.renderer, gameData.menuData.backButton);
            
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

    drawText(gameData.renderer, gameData.fpsText);

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
