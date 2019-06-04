#include "Map.h"

const char *MAP_BASE_PATH = "Data/Maps/";

Map::Map(int id) {
	_isLoaded = load(id);
}

bool Map::load(int id) {
	std::string path = MAP_BASE_PATH + std::to_string(id) + ".txt";
	FileReader file(path.c_str(), false);

	if (!file.read()) {
		printf("Couldn't load file for Map %d", id);
		return false;
	}

	if (file.exists("Tiles")) {
		for (unsigned int i = 0; i < file.get_string("Tiles").size(); i++) {
		}
	}

	return true;

	/*
	for (unsigned int i = 0; i < file.size(); i++) {
		if (file.first_string(i) == "Tiles") {
			for (unsigned int k = 0; k < file.second_string.size(); k++) {
				_tiles.push_back(file.second_string[k]);
			}
		}
	}
	*/
}