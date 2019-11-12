#include <chrono>

#include "states/gameplay.cpp"
#include "states/mainmenu.cpp"
#include "states/gameover.cpp"
#include "settings.cpp"
#include "gui.cpp"
#include "entity.cpp"
#include "breakout.cpp"

int main(int argc, char* argv[])
{
	GameData gameData = init();
	gameInit(gameData);

	SDL_SetWindowIcon(gameData.window, IMG_Load("res/breakoutlogo.png"));

	unsigned int frameCounter = 0;
	std::chrono::high_resolution_clock::time_point lastTime = std::chrono::high_resolution_clock::now();

	while (gameData.running)
	{
		// Calculates delta time
		std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> diff = currentTime - lastTime;
		gameData.deltaTime = diff.count() / 1000.0;
		lastTime = currentTime;

		// Text for frame rate
		frameCounter += 1;
		if (frameCounter % 20 == 0)
		{
			char newFpsText[256];
			sprintf_s(newFpsText, 256, "Breakout V0.1.0 | %dFPS", (int) (1.0 / gameData.deltaTime));
			gameData.fpsText.text = newFpsText;
			updateTextTexture(gameData.renderer, gameData.fpsText);

			frameCounter = 0;
		}

		gameData.running = gameHandleEvents(gameData);

		switch (gameData.gameState)
		{
			case GameState::Gameplay: 
			{
				if (gameData.running) gameData.running = gameplayUpdate(gameData);
				gameplayDraw(gameData);
			} break;

			case GameState::MainMenu:
			{
				mainMenuDraw(gameData);
			} break;

			case GameState::GameOver:
			{
				gameOverDraw(gameData);
			}
		}
	}

	// TODO(fkp): Maybe put this on menu exit?
	writeSettingsToFile(gameData.settings);
	
	return 0;
}
