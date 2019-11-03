#include <SDL/SDL.h>

#include "entity.h"
#include "breakout.h"

void ballReset(SDL_Renderer* renderer, Ball& ball, const char* filepath)
{
	if (ball.texture.texture)
	{
		SDL_DestroyTexture(ball.texture.texture);
		ball.texture.texture = nullptr;
	}

	ball.texture = createTexture(renderer, filepath);
	ball.texture.rect = SDL_Rect { (SCREEN_WIDTH - BALL_WIDTH) / 2, (SCREEN_HEIGHT  - BALL_WIDTH) / 2, BALL_WIDTH, BALL_WIDTH };
	
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

	if (ball.texture.rect.y + BALL_WIDTH > SCREEN_HEIGHT)
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
	paddle.texture = createTexture(renderer, "res/paddles/default_paddle.png");
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

	for (int brickX = 0; brickX < NUM_BRICKS_X_AXIS; brickX++)
	{
		for (int brickY = 0; brickY < NUM_BRICKS_Y_AXIS; brickY++)
		{
			Brick& brick = gameData.bricks[brickY * NUM_BRICKS_X_AXIS + brickX];

			// TODO(fkp): Do we need to store the colour in Brick?
			const char* brickTexturePath = "res/bricks/brick_white.png";

			switch (brickY)
			{
				case 0:
				{
					brickTexturePath = "res/bricks/brick_red.png";
				} break;

				case 1:
				{
					brickTexturePath = "res/bricks/brick_green.png";
				} break;

				case 2:
				{
					brickTexturePath = "res/bricks/brick_blue.png";
				} break;
				
				default:
				{
					printf("Brick texture not in rows.\n");
				} break;
			}

			brick.texture = createTexture(gameData.renderer, brickTexturePath);
			brick.texture.rect = { brickX * BRICK_WIDTH, (brickY * BRICK_HEIGHT) + BRICK_Y_OFFSET, BRICK_WIDTH, BRICK_HEIGHT };
		}
	}
}

void arrowReset(SDL_Renderer* renderer, Texture& arrow, const char* texturePath, int x, int y)
{
	arrow = createTexture(renderer, texturePath);
	arrow.rect.w = MENU_CUSTOMISE_BALL_NOT_IN_VIEW_WIDTH / 2;
	arrow.rect.h = MENU_CUSTOMISE_BALL_NOT_IN_VIEW_WIDTH;
	arrow.rect.x = x;
	arrow.rect.y = y;
}
