#include "Map.h"

#include "FileReader.h"

#define FILE_MAP_WIDTH "iwidth"
#define FILE_MAP_HEIGHT "iheight"

const char *MAP_BASE_PATH = "Data/Maps/";

Map::Map(int id) :
	_id				( id ),
	_width			( 0 ),
	_height			( 0 ),
	_pos			( { 0, 0, 0, 0 } ),
	_isLoaded		( load(id) )
{}

bool Map::load(int id) {
	std::string path = MAP_BASE_PATH + std::to_string(id) + ".txt";
	FileReader file(path.c_str(), false);

	if (!file.read()) {
		printf("Couldn't load file for Map %d", id);
		return false;
	}

	_id = id;
	_tiles.clear();

	if (file.exists(FILE_MAP_WIDTH))		 _width = file.get_int(FILE_MAP_WIDTH);
	if (file.exists(FILE_MAP_HEIGHT))		 _height = file.get_int(FILE_MAP_HEIGHT);

	if (file.exists("Tiles")) {
		std::istringstream stream(file.get_string("Tiles"));
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

	_pos.w = _width * TILE_WIDTH;
	_pos.h = _height * TILE_HEIGHT;

	return true;
}