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

	for (auto& pair : result.settings)
	{
		printf("Key: %s, value: %s\n", pair.first.c_str(), pair.second.c_str());
	}

	settingsFile.close();

	return result;
}
