#pragma once

constexpr double PI = 3.14159265358979323846;

constexpr int SCREEN_WIDTH = 960;
constexpr int SCREEN_HEIGHT = 540;

constexpr int PADDLE_WIDTH = SCREEN_WIDTH / 6;
constexpr int PADDLE_HEIGHT = SCREEN_HEIGHT / 30;
constexpr float PADDLE_VELOCITY = SCREEN_WIDTH / 2.0f;

constexpr int BALL_WIDTH = SCREEN_WIDTH / 50;
constexpr int BALL_HEIGHT = BALL_WIDTH;
constexpr float BALL_VELOCITY = SCREEN_WIDTH / 1.5f;

constexpr int BRICK_WIDTH = SCREEN_WIDTH / 6;
constexpr int BRICK_HEIGHT = SCREEN_HEIGHT / 15;

// Text properties
constexpr char ARIAL_FONT_PATH[] = "res/arial.ttf";
