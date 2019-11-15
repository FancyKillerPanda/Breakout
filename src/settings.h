#pragma once

#include <unordered_map>

// NOTE(fkp): This Settings struct is to be used mostly for saving
// the game state, and for loading the saved state at startup.
// For example, saving the ball type that was last chosen and using
// that next time.
struct Settings
{
	// Put default settings in here
	std::unordered_map<std::string, std::string> settings = {
		{ "BALL_TEXTURE_PATH", "res/balls/default_ball.png" },
		{ "HIGH_SCORE", "0" },
	};
};


Settings loadSettings();
void writeSettingsToFile(const Settings& settings);
const char* getSettingsValue(const Settings& settings, const char* key);
void setSettingsValue(Settings& settings, const char* key, const char* value);
