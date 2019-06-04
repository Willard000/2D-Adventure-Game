#include <vector>
#include <string>

#include "FileReader.h"

#ifndef MAP_H
#define MAP_H

#define TILE_WIDTH 32
#define TILE_HEIGHT 32

extern const char *MAP_BASE_PATH;

class Map {
public:
	Map(int id);

	bool load(int id);
private:
	std::vector<int> _tiles;

	int _id;
	bool _isLoaded;
};

#endif