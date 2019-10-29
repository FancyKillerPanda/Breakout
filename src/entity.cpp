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

void paddleReset(SDL_Renderer* renderer, Paddle& paddle)
{
	paddle.texture = createTexture(renderer, "res/paddle_white.png");
	paddle.texture.rect = SDL_Rect { (SCREEN_WIDTH - PADDLE_WIDTH) / 2, (SCREEN_HEIGHT * 9 / 10) - (PADDLE_HEIGHT / 2), PADDLE_WIDTH, PADDLE_HEIGHT };
	paddle.velocity.x = 0;
	paddle.velocity.y = 0;
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
	
	int brickX = SCREEN_WIDTH / 24;
	int brickY = SCREEN_HEIGHT / 10;

	for (Brick& brick : gameData.bricks)
	{
		brick.texture = createTexture(gameData.renderer, "res/brick_white.png");
		brick.texture.rect = { brickX, brickY, BRICK_WIDTH, BRICK_HEIGHT };

		brickX += BRICK_WIDTH + (SCREEN_WIDTH / 12);
	}
}
