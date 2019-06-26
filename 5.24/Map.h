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

struct Map_Surface {
	SDL_Surface *surfaces;
	int size;
};

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
	int getWidth() { return _rect.w; }
	// in pixels
	int getHeight() { return _rect.h; }

	SDL_Rect get_rect() { return _rect; }

	friend class ResourceManager;
private:
	std::string get_path(int id);
private:
	std::vector<Tile> _tiles;

	int _id;
	int _width, _height;
	SDL_Rect _rect;
	std::string _name;
	bool _isLoaded;
};

#endif