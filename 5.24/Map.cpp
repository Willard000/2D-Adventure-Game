#include "Map.h"

#include <sstream>
#include <fstream>

#include "FileReader.h"

#include "Environment.h"
#include "Log.h"

#define FILE_MAP_WIDTH "iwidth"
#define FILE_MAP_HEIGHT "iheight"
#define FILE_MAP_TILES "Tiles"

const char *MAP_BASE_PATH = "Data/Maps/";

Map::Map() :
	_id				( 0 ),
	_name			( "untitled" ),
	_width			( 0 ),
	_height			( 0 ),
	_rect			( { 0, 0, 0, 0 } ),
	_isLoaded		( false )
{}

bool Map::load(int id) {
	Environment::get().getLog()->print("Loading map - " + std::to_string(id));
	std::string path = get_path(id);
	FileReader file(path.c_str(), false);

	if (!file.read()) {
		Environment::get().getLog()->print("Could not load map - " + std::to_string(id));
		printf("Couldn't load file for Map %d", id);
		return false;
	}

	if (_id != 0) {
		save(); // save current map
	}

	int width, height;

	if (file.exists(FILE_MAP_WIDTH))		 width = file.get_int(FILE_MAP_WIDTH);
	if (file.exists(FILE_MAP_HEIGHT))		 height = file.get_int(FILE_MAP_HEIGHT);

	if (width > MAP_MAX_WIDTH) {
		Environment::get().getLog()->print("Map exceeds max width of 256 - " + std::to_string(_width));
		return false;
	}
	_width = width;

	if (height > MAP_MAX_HEIGHT) {
		Environment::get().getLog()->print("Map exceeds max height of 256" + std::to_string(_height));
		return false;
	}
	_height = height;

	_id = id;
	_tiles.clear();

	if (file.exists(FILE_MAP_TILES)) {
		std::istringstream stream(file.get_string(FILE_MAP_TILES));
		Tile tile;
		int x = 0, y = 0;
		int width = 1, height = 1;
		while (stream >> tile.id) {
			tile.pos.x = x;
			tile.pos.y = y;
			x += TILE_WIDTH;
			width++;
			if (width > _width) {
				x = 0;
				y += TILE_WIDTH;
				width = 1;
				height++;
			}
			_tiles.push_back(tile);
		}

		int size = _width * _height;
		if ((int)_tiles.size() < size) {
			printf("Map %d - Too few tiles, loaded: %d, needed: %d", _id, _tiles.size(), size);
		}
		else if ((int)_tiles.size() > size) {
			printf("Map %d - Too many tiles, loaded: %d, needed: %d", _id, _tiles.size(), size);
		}
	}

	_rect.w = _width * TILE_WIDTH;
	_rect.h = _height * TILE_HEIGHT;

	Environment::get().getLog()->print("Map Loaded");

	return true;
}

void Map::save() {
	Environment::get().getLog()->print("Saving map - " + std::to_string(_id));
	std::string path = get_path(_id);

	std::ofstream file(path.c_str(), std::ios::out, std::ios::trunc);

	file << "# Map" << _id << std::endl;
	file << FILE_MAP_WIDTH << " " << _width << std::endl;
	file << FILE_MAP_HEIGHT << " " << _height << std::endl;
	file << FILE_MAP_TILES;
	for (unsigned int i = 0; i < _tiles.size(); i++) {
		file << " " << _tiles[i].id;
	}
	file << std::endl;
}

bool Map::create_new(int id, std::string name, int width, int height, int base_tile_id) {
	std::string path = get_path(id);

	Environment::get().getLog()->print(
		"Creating new map:"
		+ std::string("\n id: ")
		+ std::to_string(id)
		+ std::string("\n name: ")
		+ name
		+ std::string("\n width: ")
		+ std::to_string(width)
		+ std::string("\n height: ")
		+ std::to_string(height)
		+ std::string("\n base_tile_id: ")
		+ std::to_string(base_tile_id)
	);

	if (file_exists(path.c_str())) {
		Environment::get().getLog()->print("Failed to create map - file already exists");
		return false;
	}

	save(); // current map

	_id = id;
	_name = name;
	_tiles.clear();
	_width = width;
	_height = height;
	_rect.w = _width * TILE_WIDTH;
	_rect.h = _height * TILE_HEIGHT;

	Tile tile;
	tile.id = base_tile_id;
	tile.pos.w = TILE_WIDTH;
	tile.pos.h = TILE_HEIGHT;

	for (unsigned int i = 0; i < unsigned int(_width * _height); i++) {
		tile.pos.x = i * TILE_WIDTH;
		tile.pos.y = ((i / _width) + 1) * TILE_HEIGHT;
		_tiles.push_back(tile);
	}

	save(); // new created map

	return true;
}

std::string Map::get_path(int id) {
	return MAP_BASE_PATH + std::to_string(id) + ".txt";
}