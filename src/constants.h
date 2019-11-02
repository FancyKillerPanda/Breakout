#pragma once

constexpr double PI = 3.14159265358979323846;

constexpr int SCREEN_WIDTH = 960;
constexpr int SCREEN_HEIGHT = 540;

// Gameplay properties
constexpr int PADDLE_WIDTH = SCREEN_WIDTH / 6;
constexpr int PADDLE_HEIGHT = SCREEN_HEIGHT / 30;
constexpr float PADDLE_VELOCITY = SCREEN_WIDTH / 2.0f;

constexpr int BALL_WIDTH = SCREEN_WIDTH / 50;
constexpr float BALL_VELOCITY = SCREEN_WIDTH / 1.3f;
constexpr float BALL_ROTATION_SPEED = 3.0f;  // Degrees per frame

constexpr int BRICK_WIDTH = SCREEN_WIDTH / 10;
constexpr int BRICK_HEIGHT = SCREEN_HEIGHT / 15;
constexpr int BRICK_Y_OFFSET = SCREEN_HEIGHT / 15;
constexpr int NUM_BRICKS_X_AXIS = 10;
constexpr int NUM_BRICKS_Y_AXIS = 3;

// Text properties
const int NUM_ITEMS_IN_MENU = 3;
const SDL_Color MENU_COLOURS[3] = { {255, 255, 255, 255}, {255, 0, 0, 255}, {192, 192, 192, 255} };
constexpr char ARIAL_FONT_PATH[] = "res/fonts/arial.ttf";
constexpr char BAD_SIGNAL_FONT_PATH[] = "res/fonts/Bad-Signal.ttf";

// Menu properties
constexpr int MENU_CUSTOMISE_BALL_IN_VIEW_WIDTH = 75;
constexpr int MENU_CUSTOMISE_BALL_NOT_IN_VIEW_WIDTH = 40;
constexpr int MENU_CUSTOMISE_BALL_PREV_CENTER_X = SCREEN_WIDTH * 10 / 18;
constexpr int MENU_CUSTOMISE_BALL_PREV_CENTER_Y = SCREEN_HEIGHT / 3;
constexpr int MENU_CUSTOMISE_BALL_IN_VIEW_CENTER_X = SCREEN_WIDTH * 12 / 18;
constexpr int MENU_CUSTOMISE_BALL_IN_VIEW_CENTER_Y = SCREEN_HEIGHT / 3;
constexpr int MENU_CUSTOMISE_BALL_NEXT_CENTER_X = SCREEN_WIDTH * 14 / 18;
constexpr int MENU_CUSTOMISE_BALL_NEXT_CENTER_Y = SCREEN_HEIGHT / 3;
