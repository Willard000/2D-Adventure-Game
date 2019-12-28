#include <string>

#ifndef GLOBALS_H
#define GLOBALS_H

#define SYSTEM_FILE "Data/system.txt"

#define TEXTURE_FILE "Data/Textures/textures.txt"
#define SURFACE_FILE "Data/Textures/surfaces.txt"

#define ENTITY_FOLDER "Data/Entities/"

#define PLAYER_FILE "Data/player.txt"

#define QUEST_FOLDER "Data/Quests"

enum {
	MOVE_UP = 1,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UPLEFT,
	MOVE_UPRIGHT,
	MOVE_DOWNLEFT,
	MOVE_DOWNRIGHT
};

enum {
	TYPE_TILE,

	TYPE_PLAYER,
	TYPE_OBJECT,
	TYPE_SPELL,
	TYPE_ENEMY,
	TYPE_EFFECT,
	TYPE_ITEM,
	TYPE_NPC,


	// ^ have textures ^
	TOTAL_TEXTURE_TYPES,
	// v dont have textures v

	TYPE_SOLID,
	TYPE_WARP,
	TYPE_SELECT,
};

#define TYPE_EX_ICON 10

const std::string STYPE_TILE = "Tile";

const std::string STYPE_PLAYER = "Player";
const std::string STYPE_OBJECT = "Object";
const std::string STYPE_SPELL = "Spell";
const std::string STYPE_ENEMY = "Enemy";
const std::string STYPE_EFFECT = "Effect";
const std::string STYPE_ITEM = "Item";
const std::string STYPE_NPC = "NPC";

const std::string STYPE_SOLID = "Solid";
const std::string STYPE_WARP = "Warp";
const std::string STYPE_SELECT = "Select";

const std::string STYPE_NULL = "";

const inline std::string &STYPE(const int &type) {
	switch (type) {
	case TYPE_PLAYER:		return STYPE_PLAYER;
	case TYPE_OBJECT:		return STYPE_OBJECT;
	case TYPE_SPELL:		return STYPE_SPELL;
	case TYPE_ENEMY:		return STYPE_ENEMY;
	case TYPE_EFFECT:		return STYPE_EFFECT;
	case TYPE_ITEM:			return STYPE_ITEM;
	case TYPE_NPC:			return STYPE_NPC;
	case TYPE_SOLID:		return STYPE_SOLID;
	case TYPE_WARP:			return STYPE_WARP;
	case TYPE_SELECT:		return STYPE_SELECT;
	case TYPE_TILE:			return STYPE_TILE;
	}

	return STYPE_NULL;
}

#endif