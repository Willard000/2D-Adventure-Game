#include <vector>
#include <string>
#include <sstream>

#include <SDL.h>

#include "FileReader.h"

#ifndef MAP_H
#define MAP_H

#define FILE_MAP_WIDTH "iwidth"
#define FILE_MAP_HEIGHT "iheight"

#define TYPE_TILE "Tile"

#define TILE_WIDTH 32
#define TILE_HEIGHT 32

extern const char *MAP_BASE_PATH;

struct Tile {
	int id = 0;
	SDL_Rect pos = { 0, 0, TILE_WIDTH, TILE_HEIGHT };
};

class Map {
public:
	Map(int id);

	bool load(int id);

	friend class ResourceManager;
private:
	std::vector<Tile> _tiles;

	int _id;
	int _width, _height;
	SDL_Rect _pos;
	bool _isLoaded;
};

#endif