#include <SDL/SDL.h>

#include "entity.h"
#include "breakout.h"

void ballReset(SDL_Renderer* renderer, Ball& ball)
{
	if (ball.texture.texture)
	{
		SDL_DestroyTexture(ball.texture.texture);
		ball.texture.texture = nullptr;
	}

	ball.texture = createTexture(renderer, "res/ball.png");
	ball.texture.rect = SDL_Rect { (SCREEN_WIDTH - BALL_WIDTH) / 2, (SCREEN_HEIGHT  - BALL_HEIGHT) / 2, BALL_WIDTH, BALL_HEIGHT };
	
	ball.velocity.x = 0;
	ball.velocity.y = BALL_VELOCITY;
}

// NOTE(fkp): Returns false if game over
bool ballUpdate(GameData& gameData, Ball& ball)
{
	// Ball bouncing off side of window
	if (ball.texture.rect.x < 0)
	{
		ball.texture.rect.x = 0;
		ball.velocity.x *= -1;
	}
	else if (ball.texture.rect.x + BALL_WIDTH > SCREEN_WIDTH)
	{
		ball.texture.rect.x = SCREEN_WIDTH - BALL_WIDTH;
		ball.velocity.x *= -1;
	}
	
	// Ball bouncing off top of window
	if (ball.texture.rect.y < 0)
	{
		ball.texture.rect.y = 0;
		ball.velocity.y *= -1;
	}

	if (ball.texture.rect.y + BALL_HEIGHT > SCREEN_HEIGHT)
	{
		return false;
	}

	// Ball rotation
	if (ball.velocity.x >= 0.0f)
	{
		ball.rotationAngle += BALL_ROTATION_SPEED;
	}
	else
	{
		ball.rotationAngle -= BALL_ROTATION_SPEED;
	}

	return true;
}

void paddleReset(SDL_Renderer* renderer, Paddle& paddle)
{
	paddle.texture = createTexture(renderer, "res/paddle_white.png");
	paddle.texture.rect = SDL_Rect { (SCREEN_WIDTH - PADDLE_WIDTH) / 2, (SCREEN_HEIGHT * 9 / 10) - (PADDLE_HEIGHT / 2), PADDLE_WIDTH, PADDLE_HEIGHT };
	paddle.velocity.x = 0;
	paddle.velocity.y = 0;
}

void paddleUpdate(GameData& gameData, Paddle& paddle)
{
	// Left/right bounds checking for paddle
	if (paddle.texture.rect.x < 0)
	{
		paddle.texture.rect.x = 0;
	}

	if (paddle.texture.rect.x + PADDLE_WIDTH > SCREEN_WIDTH)
	{
		paddle.texture.rect.x = SCREEN_WIDTH - PADDLE_WIDTH;
	}

	paddle.texture.rect.x += (int) (paddle.velocity.x * gameData.deltaTime);
}

void bricksReset(GameData& gameData)
{
	for (Brick& brick : gameData.bricks)
	{
		if (brick.texture.texture)
		{
			destroyTexture(brick.texture);
		}

		brick = {};
	}

	for (int brickX = 0; brickX < 10; brickX++)
	{
		for (int brickY = 0; brickY < 3; brickY++)
		{
			Brick& brick = gameData.bricks[brickY * 10 + brickX];

			brick.texture = createTexture(gameData.renderer, "res/brick_white.png");
			brick.texture.rect = { brickX * BRICK_WIDTH, (brickY * BRICK_HEIGHT) + BRICK_Y_OFFSET, BRICK_WIDTH, BRICK_HEIGHT };
		}
	}
}
