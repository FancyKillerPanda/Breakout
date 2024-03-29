#include <SDL/SDL.h>

#include "state.h"

void gameplayInit(GameData& gameData)
{
	if (gameData.gameplayInitialised)
	{
		paddleReset(gameData.renderer, gameData.paddle);
		ballReset(gameData.renderer, gameData.ball, getSettingsValue(gameData.settings, "BALL_TEXTURE_PATH"));
		bricksReset(gameData);
		return;
	}
	
	gameData.pausedText = createText(gameData.renderer, "PAUSED", 64, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, DIGITAL_DISCO_FONT_PATH, SDL_Color { 255, 0, 0, 255 });

	paddleReset(gameData.renderer, gameData.paddle);
	ballReset(gameData.renderer, gameData.ball, getSettingsValue(gameData.settings, "BALL_TEXTURE_PATH"));
	bricksReset(gameData);
	
	gameData.gameplayInitialised = true;
}

// Called each time entering the state
void gameplayOnEnter(GameData& gameData)
{
	ballReset(gameData.renderer, gameData.ball, getSettingsValue(gameData.settings, "BALL_TEXTURE_PATH"));
	gameData.score = 0;
}

// NOTE(fkp): Returns true if success, false if games needs to exit
bool gameplayHandleEvents(GameData& gameData)
{
	if (!gameData.gameplayInitialised)
	{
		printf("Gameplay not initialised, cannot handle events.\n");
		return false;
	}
	
	switch (gameData.event.type)
	{
		case SDL_KEYDOWN:
		{
			switch (gameData.event.key.keysym.sym)
			{
				// Toggles paused
				case SDLK_ESCAPE:
				{
					if (gameData.paused)
					{
						return false;
					}

					gameData.paused = true;
				} break;
				
				case SDLK_p:
				{
					gameData.paused = !gameData.paused;
				} break;

				case SDLK_RETURN:
				{
					if (gameData.paused)
					{
						gameData.paused = false;
					}
				} break;
			}
		} break;
	}

	// Stops the rest of the function if paused
	if (gameData.paused)
	{
		return true;
	}

	gameData.paddle.velocity.x = 0;

	if (gameData.keyboardState[SDL_SCANCODE_RIGHT])
	{
		gameData.paddle.velocity.x += PADDLE_VELOCITY;
	}

	if (gameData.keyboardState[SDL_SCANCODE_LEFT])
	{
		gameData.paddle.velocity.x -= PADDLE_VELOCITY;
	}

	return true;
}

// NOTE(fkp): Returns true if success, false if games needs to exit
bool gameplayUpdate(GameData& gameData)
{
	if (!gameData.gameplayInitialised)
	{
		printf("Gameplay not initialised, cannot update.\n");
		return false;
	}
	
	if (gameData.paused)
	{
		return true;
	}
	
	if (SDL_HasIntersection(&gameData.ball.texture.rect, &gameData.paddle.texture.rect))
	{
		const float maxBallBounceAngle = 75;
		float ballCenterX = gameData.ball.texture.rect.x + (BALL_WIDTH / 2.0f);
		float paddleCenterX = gameData.paddle.texture.rect.x + (PADDLE_WIDTH / 2.0f);
		float maxTouchingDistance = (PADDLE_WIDTH / 2.0f) + (BALL_WIDTH / 2.0f);
		float ballAngle = (((ballCenterX - paddleCenterX) / maxTouchingDistance) * maxBallBounceAngle) + 90;
		gameData.ball.velocity.x = (float) -cos(ballAngle * PI / 180) * BALL_VELOCITY;
		gameData.ball.velocity.y = (float) -sin(ballAngle * PI / 180) * BALL_VELOCITY;
	}

	// TODO(fkp): Can there be more than one brick hit?
	// The brick that was hit
	Brick* brickHit = nullptr;

	// Moves ball on x-axis and checks for collision
	gameData.ball.texture.rect.x += (int) (gameData.ball.velocity.x * gameData.deltaTime);

	for (Brick& brick : gameData.bricks)
	{
		if (brick.numOfHitsLeft > 0 && SDL_HasIntersection(&gameData.ball.texture.rect, &brick.texture.rect))
		{
			// Moves back outside the rect
			gameData.ball.texture.rect.x -= (int) (gameData.ball.velocity.x * gameData.deltaTime);
			// Switches x-axis direction
			gameData.ball.velocity.x *= -1;

			brickHit = &brick;
		}
	}

	// Moves ball on y-axis and checks for collision
	gameData.ball.texture.rect.y += (int) (gameData.ball.velocity.y * gameData.deltaTime);

	for (Brick& brick : gameData.bricks)
	{
		if (brick.numOfHitsLeft > 0 && SDL_HasIntersection(&gameData.ball.texture.rect, &brick.texture.rect))
		{
			// Moves back outside the rect
			gameData.ball.texture.rect.y -= (int) (gameData.ball.velocity.y * gameData.deltaTime);
			// Switches y-axis direction
			gameData.ball.velocity.y *= -1;

			brickHit = &brick;
		}
	}

	// Removes one hit from brick
	if (brickHit)
	{	
		// TODO(fkp): Change brick texture
		brickHit->numOfHitsLeft -= 1;

		// Destroys brick
		if (brickHit->numOfHitsLeft == 0)
		{
			destroyTexture(brickHit->texture);
			gameData.score += 10;
		}
	}

	paddleUpdate(gameData, gameData.paddle);

	if (!ballUpdate(gameData, gameData.ball))
	{
		// NOTE(fkp): Game over
		changeState(gameData, GameState::GameOver);
	}

	return true;
}

// Draws the game state
void gameplayDraw(GameData& gameData)
{
	if (!gameData.gameplayInitialised)
	{
		printf("Gameplay not initialised, cannot draw.\n");
		return;
	}
	
	SDL_SetRenderDrawColor(gameData.renderer, 0, 0, 0, 255);
	SDL_RenderClear(gameData.renderer);

	for (Brick& brick : gameData.bricks)
	{
		if (brick.numOfHitsLeft == 0)
		{
			continue;
		}
		
		drawTexture(gameData.renderer, brick.texture);
	}

	drawTexture(gameData.renderer, gameData.ball.texture, (double) gameData.ball.rotationAngle);
	drawTexture(gameData.renderer, gameData.paddle.texture);

	// Paused text
	if (gameData.paused)
	{
		// Paused dim
		SDL_Rect dimRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		SDL_SetRenderDrawColor(gameData.renderer, 0, 0, 0, 170);
		SDL_RenderFillRect(gameData.renderer, &dimRect);
		
		drawText(gameData.renderer, gameData.pausedText);

	}

	drawText(gameData.renderer, gameData.fpsText);

	SDL_RenderPresent(gameData.renderer);
}
