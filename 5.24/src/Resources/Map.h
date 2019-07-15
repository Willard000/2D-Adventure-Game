#include <vector>
#include <string>
#include <map>

#include <SDL.h>

#include "Entity.h"
#include "FileReader.h"
#include "Quadtree.h"

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

	struct Warp {
		int from_id = 0, to_id = 0;
		SDL_Rect from = { 0, 0, 0, 0 };
		SDL_Rect to = { 0, 0, 0, 0 };
	};

	Map();

	bool load(int id);
	void save();
	bool create_new(int id, std::string name, int width, int height, int base_tile_id);

	bool solid_collision(const SDL_Rect &pos);
	Warp *warp_collision(const SDL_Rect &pos);
	Entity *entity_collision(const SDL_Rect &pos);

	void add_solid(int index);
	void remove_solid(int index);

	void add_warp(Warp warp);
	void remove_warp(Warp *warp);
	void remove_warp(int index);

	int get_id() { return _id; }
	// in pixels
	int get_width() { return _width; }
	// in pixels
	int get_height() { return _height; }

	SDL_Rect get_rect() { return _rect; }

	void test();

	void draw_warps();
	void draw_solids();

	std::vector<Warp> *get_warps() { return &_warps; }

	friend class ResourceManager;
private:
	std::string get_path(int id);
	void load_tiles(FileReader &file);
	void load_solids(FileReader &file);
	void load_warps(FileReader &file);
	void load_entities(FileReader &file);

	void save_entities(std::ostream &file);
private:
	std::vector<Tile> _tiles;

	std::map<int, SDL_Rect> _solids;
	QuadTree<int> _solids_tree;

	std::vector<Warp> _warps;
	QuadTree<Warp *> _warps_tree;

	QuadTree<Entity *> _entities_tree;

	int _id;
	int _width, _height;
	SDL_Rect _rect;
	std::string _name;
	bool _is_loaded;
};

#endif