#include <SDL/SDL.h>

#include "entity.h"
#include "breakout.h"

void ballReset(Entity& ball)
{
	ball.colour = SDL_Colour { 255, 255, 255, 255 };
	
	ball.rect = SDL_Rect { (SCREEN_WIDTH - BALL_WIDTH) / 2, (SCREEN_HEIGHT  - BALL_HEIGHT) / 2, BALL_WIDTH, BALL_HEIGHT };
	ball.velocity.x = 0;
	ball.velocity.y = BALL_VELOCITY;
}

void paddleReset(Entity& paddle)
{
	paddle.colour = SDL_Colour { 255, 255, 255, 255 };
	
	paddle.rect = SDL_Rect { (SCREEN_WIDTH - PADDLE_WIDTH) / 2, (SCREEN_HEIGHT * 9 / 10) - (PADDLE_HEIGHT / 2), PADDLE_WIDTH, PADDLE_HEIGHT };
	paddle.velocity.x = 0;
	paddle.velocity.y = 0;
}
