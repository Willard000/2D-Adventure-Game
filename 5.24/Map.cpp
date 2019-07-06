#include "Map.h"

#include <sstream>
#include <fstream>

#include "FileReader.h"

#include "Environment.h"
#include "Log.h"
#include "ResourceManager.h"

#include "Entity.h"
#include "PositionComponent.h"

#include "Collision.h"

#define FILE_MAP_WIDTH "iwidth"
#define FILE_MAP_HEIGHT "iheight"
#define FILE_MAP_TILES "Tiles"
#define FILE_MAP_SOLIDS "Solids"
#define FILE_MAP_WARPS "Warps"
#define FILE_MAP_ENTITIES "Entities"

#define FILE_WARP_FROM_ID "iwarp_from_id"
#define FILE_WARP_TO_ID "iwarp_to_id"
#define FILE_WARP_FROM_X "ifrom_x"
#define FILE_WARP_FROM_Y "ifrom_y"
#define FILE_WARP_FROM_W "ifrom_w"
#define FILE_WARP_FROM_H "ifrom_h"
#define FILE_WARP_TO_X "ito_x"
#define FILE_WARP_TO_Y "ito_y"
#define FILE_WARP_TO_W "ito_w"
#define FILE_WARP_TO_H "ito_h"

#define FILE_ENTITY_TYPE "stype"
#define FILE_ENTITY_TYPE_ID "itype_id"
#define FILE_ENTITY_POSITION_X "dposition_x"
#define FILE_ENTITY_POSITION_Y "dposition_y"

#define FILE_MAP_SEPERATOR "."

const char *MAP_BASE_PATH = "Data/Maps/";

void save_entities(std::ostream &file) {
	file << FILE_MAP_ENTITIES << " ";
	std::map<int, Entity *> *entities = Environment::get().get_resource_manager()->get_entities();
	for (auto it = entities->begin(); it != entities->end(); it++) {
		file << FILE_ENTITY_TYPE << " " << it->second->get_type() << " "
			 << FILE_ENTITY_TYPE_ID << " " << it->second->get_type_id() << " ";

		if (PositionComponent *position = GetPosition(it->second)) {
			file << FILE_ENTITY_POSITION_X << " " << position->pos_x << " "
				 << FILE_ENTITY_POSITION_Y << " " << position->pos_y << " ";
		}

		file << FILE_MAP_SEPERATOR << " ";
	}
}

void load_entities(FileReader &file) {
	if (!file.exists(FILE_MAP_ENTITIES)) {
		return;
	}

	Environment::get().get_resource_manager()->clear_entities();

	std::stringstream stream(file.get_string(FILE_MAP_ENTITIES));
	std::string key, data;
	std::string type = "Object";
	int type_id = 0;
	double position_x = 0, position_y = 0;
	while ((stream >> key)) {
		if (key == FILE_MAP_SEPERATOR) {
			Entity *entity = new Entity(type, type_id);
			if (PositionComponent *position = GetPosition(entity)) {
				position->pos_x = position_x;
				position->pos_y = position_y;
				position->rect.x = (int)position_x;
				position->rect.y = (int)position_y;
			}
			Environment::get().get_resource_manager()->add(entity);
		}
		else {
			stream >> data;
			if (key == FILE_ENTITY_TYPE) type = data;
			else if (key == FILE_ENTITY_TYPE_ID) type_id = std::stoi(data);
			else if (key == FILE_ENTITY_POSITION_X) position_x = std::stod(data);
			else if (key == FILE_ENTITY_POSITION_Y) position_y = std::stod(data);
		}
	}
}

Map::Map() :
	_id				( 0 ),
	_name			( "untitled" ),
	_width			( 0 ),
	_height			( 0 ),
	_rect			( { 0, 0, 0, 0 } ),
	_is_loaded		( false )
{}

bool Map::load(int id) {
	Environment::get().get_log()->print("Loading map - " + std::to_string(id));
	std::string path = get_path(id);
	FileReader file(path.c_str(), false);

	if (!file.read()) {
		Environment::get().get_log()->print("Could not load map - " + std::to_string(id));
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
		Environment::get().get_log()->print("Map exceeds max width of 256 - " + std::to_string(_width));
		return false;
	}
	_width = width;

	if (height > MAP_MAX_HEIGHT) {
		Environment::get().get_log()->print("Map exceeds max height of 256" + std::to_string(_height));
		return false;
	}
	_height = height;

	_id = id;

	load_tiles(file);
	load_solids(file);
	load_warps(file);
	load_entities(file);

	_rect.w = _width * TILE_WIDTH;
	_rect.h = _height * TILE_HEIGHT;

	Environment::get().get_log()->print("Map Loaded");

	return true;
}

void Map::save() {
	Environment::get().get_log()->print("Saving map - " + std::to_string(_id));
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

	file << FILE_MAP_SOLIDS;
	for (auto it = _solids.begin(); it != _solids.end(); it++) {
		file << " " << it->first;
	}
	file << std::endl;

	file << FILE_MAP_WARPS << " ";
	for (auto it = _warps.begin(); it != _warps.end(); it++) {
		file
			<< FILE_WARP_FROM_ID << " " << it->from_id
			<< " " << FILE_WARP_TO_ID << " " << it->to_id
			<< " " << FILE_WARP_FROM_X << " " << it->from.x
			<< " " << FILE_WARP_FROM_Y << " " << it->from.y
			<< " " << FILE_WARP_FROM_W << " " << it->from.w
			<< " " << FILE_WARP_FROM_H << " " << it->from.h
			<< " " << FILE_WARP_TO_X << " " << it->to.x
			<< " " << FILE_WARP_TO_Y << " " << it->to.y
			<< " " << FILE_WARP_TO_W << " " << it->to.w
			<< " " << FILE_WARP_TO_H << " " << it->to.h
			<< " " << FILE_MAP_SEPERATOR << " ";
	}
	file << std::endl;

	save_entities(file);
}

bool Map::create_new(int id, std::string name, int width, int height, int base_tile_id) {
	std::string path = get_path(id);

	Environment::get().get_log()->print(
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
		Environment::get().get_log()->print("Failed to create map - file already exists");
		return false;
	}

	save(); // current map

	_tiles.clear();
	_solids.clear();
	_warps.clear();

	Environment::get().get_resource_manager()->clear_entities();

	_id = id;
	_name = name;
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

bool Map::solid_collision(const SDL_Rect &pos) {
	for (auto it = _solids.begin(); it != _solids.end(); it++) {
		if (collision(pos, it->second)) {
			return true;
		}
	}
	return false;
}


Map::Warp *Map::warp_collision(const SDL_Rect &pos) {
	for (auto it = _warps.begin(); it != _warps.end(); it++) {
		if (collision(pos, it->from)) {
			return &(*it);
		}
	}
	return nullptr;
}

void Map::add_warp(Warp warp) {
	_warps.push_back(warp);
}

void Map::remove_warp(Warp *warp) {
	for (auto it = _warps.begin(); it != _warps.end(); it++) {
		if (warp == &(*it)) {
			delete warp;
			_warps.erase(it);
		}
	}
}

std::string Map::get_path(int id) {
	return MAP_BASE_PATH + std::to_string(id) + ".txt";
}

void Map::load_tiles(FileReader &file) {
	if (!file.exists(FILE_MAP_TILES)) {
		return;
	}

	_tiles.clear();

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
		Environment::get().get_log()->print(
			"Map" + std::to_string(_id)
			+ " - too few tiles, loaded: "
			+ std::to_string(_tiles.size())
			+ " expected: " + std::to_string(size)
		);
	}
	else if ((int)_tiles.size() > size) {
		Environment::get().get_log()->print(
			"Map" + std::to_string(_id)
			+ " - too many tiles, loaded: "
			+ std::to_string(_tiles.size())
			+ " expected: " + std::to_string(size)
		);
	}
}

void Map::load_solids(FileReader &file) {
	if (!file.exists(FILE_MAP_SOLIDS)) {
		return;
	}

	_solids.clear();

	std::istringstream stream(file.get_string(FILE_MAP_SOLIDS));
	int tile_num = 0;
	SDL_Rect solid = { 0, 0, TILE_WIDTH, TILE_HEIGHT };
	while (stream >> tile_num) {
		solid.x = (tile_num % _width) * TILE_WIDTH;
		solid.y = (tile_num / _height) * TILE_HEIGHT;
		_solids[tile_num] = solid;
	}
}

void Map::load_warps(FileReader &file) {
	if (!file.exists(FILE_MAP_WARPS)) {
		return;
	}

	_warps.clear();

	std::istringstream stream(file.get_string(FILE_MAP_WARPS));
	Warp warp;
	std::string key, data;

	while (stream >> key) {
		if (key == FILE_MAP_SEPERATOR) {
			_warps.push_back(warp);
		}
		else {
			stream >> data;
			if (key == FILE_WARP_FROM_ID) warp.from_id = std::stoi(data);
			else if (key == FILE_WARP_TO_ID) warp.to_id = std::stoi(data);
			else if (key == FILE_WARP_FROM_X) warp.from.x = std::stoi(data);
			else if (key == FILE_WARP_FROM_Y) warp.from.y = std::stoi(data);
			else if (key == FILE_WARP_FROM_W) warp.from.w = std::stoi(data);
			else if (key == FILE_WARP_FROM_H) warp.from.h = std::stoi(data);
			else if (key == FILE_WARP_TO_X) warp.to.x = std::stoi(data);
			else if (key == FILE_WARP_TO_Y) warp.to.y = std::stoi(data);
			else if (key == FILE_WARP_TO_W) warp.to.w = std::stoi(data);
			else if (key == FILE_WARP_TO_H) warp.to.h = std::stoi(data);
		}
	}

}