#pragma once

#include <unordered_map>

struct Settings
{
	// Put default settings in here
	std::unordered_map<std::string, std::string> settings = {
		
	};
};


Settings loadSettings();
