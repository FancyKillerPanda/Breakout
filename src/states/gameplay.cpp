#include <SDL/SDL.h>

#include "state.h"

// NOTE(fkp): Returns true if success, false if games needs to exit
bool gameplayHandleEvents(GameData& gameData)
{
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
	// Left/right bounds checking for paddle
	if (gameData.paddle.texture.rect.x < 0)
	{
		gameData.paddle.texture.rect.x = 0;
	}

	if (gameData.paddle.texture.rect.x + PADDLE_WIDTH > SCREEN_WIDTH)
	{
		gameData.paddle.texture.rect.x = SCREEN_WIDTH - PADDLE_WIDTH;
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

	// Moves ball on x-axis and checks for collision
	gameData.ball.texture.rect.x += (int) (gameData.ball.velocity.x * gameData.deltaTime);

	// TODO(fkp): Can there be more than one brick hit?
	// The brick that was hit
	Brick* brickHit = nullptr;

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
		}
	}

	if (!ballUpdate(gameData, gameData.ball))
	{
		// NOTE(fkp): Game over
		// TODO(fkp): Splash screen instead of just restarting ball
		ballReset(gameData.renderer, gameData.ball);
		paddleReset(gameData.renderer, gameData.paddle);
		bricksReset(gameData);
	}
	
	gameData.paddle.texture.rect.x += (int) (gameData.paddle.velocity.x * gameData.deltaTime);

	return true;
}

// Draws the game state
void gameplayDraw(GameData& gameData)
{
	SDL_SetRenderDrawColor(gameData.renderer, 0, 0, 0, 255);
	SDL_RenderClear(gameData.renderer);

	for (Brick& brick : gameData.bricks)
	{
		if (brick.numOfHitsLeft == 0)
		{
			continue;
		}
		
		SDL_RenderCopy(gameData.renderer, brick.texture.texture, nullptr, &brick.texture.rect);
	}

	SDL_RenderCopyEx(gameData.renderer, gameData.ball.texture.texture, nullptr, &gameData.ball.texture.rect, (double) gameData.ball.rotationAngle, nullptr, SDL_FLIP_NONE);
	SDL_RenderCopy(gameData.renderer, gameData.paddle.texture.texture, nullptr, &gameData.paddle.texture.rect);
	SDL_RenderCopy(gameData.renderer, gameData.fpsText.texture, nullptr, &gameData.fpsText.rect);

	SDL_RenderPresent(gameData.renderer);
}
