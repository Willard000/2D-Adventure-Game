#include <vector>
#include <string>
#include <sstream>

#include <SDL.h>

#ifndef MAP_H
#define MAP_H

#define TYPE_TILE "Tile"

#define TILE_WIDTH 32
#define TILE_HEIGHT 32

extern const char *MAP_BASE_PATH;

class Map {
public:

	struct Tile {
		int id = 0;
		SDL_Rect pos = { 0, 0, TILE_WIDTH, TILE_HEIGHT };
	};

	Map(int id);

	bool load(int id);

	int get_id() { return _id; }
	int getWidth() { return _pos.w; }
	int getHeight() { return _pos.h; }

	friend class ResourceManager;
private:
	std::vector<Tile> _tiles;

	int _id;
	int _width, _height;
	SDL_Rect _pos;
	bool _isLoaded;
};

#endif