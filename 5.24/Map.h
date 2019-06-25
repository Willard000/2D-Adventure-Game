#include <vector>
#include <string>

#include <SDL.h>

#ifndef MAP_H
#define MAP_H

#define TYPE_TILE "Tile"

#define TILE_WIDTH 32
#define TILE_HEIGHT 32

#define MAP_MAX_WIDTH 256
#define MAP_MAX_HEIGHT 256

extern const char *MAP_BASE_PATH;

class Map {
public:

	struct Tile {
		int id = 0;
		SDL_Rect pos = { 0, 0, TILE_WIDTH, TILE_HEIGHT };
	};

	Map();

	bool load(int id);
	void save();
	bool create_new(int id, std::string name, int width, int height, int base_tile_id);

	int get_id() { return _id; }
	// in pixels
	int getWidth() { return _pos.w; }
	// in pixels
	int getHeight() { return _pos.h; }

	friend class ResourceManager;
private:
	std::string get_path(int id);
private:
	std::vector<Tile> _tiles;

	int _id;
	int _width, _height;
	SDL_Rect _pos;
	std::string _name;
	bool _isLoaded;
};

#endif