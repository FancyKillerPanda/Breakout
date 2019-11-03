#include <fstream>

#include "settings.h"

Settings loadSettings()
{
	Settings result;
	
	std::string key;
	std::string value;
	
	std::ifstream settingsFile;
	settingsFile.open("res/settings.txt");

	if (!settingsFile)
	{
		printf("Settings file open failed, using default settings.\n");
		return Settings {};
	}

	while (settingsFile >> key)
	{
		if (!(settingsFile >> value))
		{
			printf("Settings file has uneven number of items, using default settings.\n");
			return Settings {};
		}

		result.settings[key] = value;
	}

	settingsFile.close();

	return result;
}

const char* getSettingsValue(const Settings& settings, const char* key)
{
	const auto& result = settings.settings.find(key);

	if (result != settings.settings.end())
	{
		return result->second.c_str();
	}

	printf("Key (%s) does not exist in settings.\n", key);
	return "";
}

void setSettingsValue(Settings& settings, const char* key, const char* value)
{
	settings.settings[key] = value;
}
