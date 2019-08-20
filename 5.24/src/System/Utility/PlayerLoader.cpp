#include "PlayerLoader.h"

#include <sstream>
#include <fstream>

#include "Environment.h"
#include "ResourceManager.h"
#include "Window.h"

#include "FileReader.h"

#include "Globals.h"

#include "Entity.h"
#include "PositionComponent.h"
#include "ItemComponent.h"
#include "PlayerComponent.h"

#define FILE_PLAYER_POSITION "position"

#define FILE_PLAYER_INVENTORY "inventory"
#define FILE_PLAYER_EQUIPPED "equipped"

#define FILE_PLAYER_NAME "sname"
#define FILE_PLAYER_LEVEL "ilevel"
#define FILE_PLAYER_EXP "iexp"
#define FILE_PLAYER_HEALTH "ihealth"
#define FILE_PLAYER_MANA "imana"
#define FILE_PLAYER_MAP_ID "imap_id"

void load_position(FileReader &file, Entity *player) {
	PositionComponent *player_position = GetPosition(player);

	float x = 0, y = 0;

	if (file.exists(FILE_PLAYER_POSITION)) {
		std::istringstream position(file.get_string(FILE_PLAYER_POSITION));
		position >> x >> y;
	}

	player_position->set(x, y);

	Environment::get().get_window()->get_camera()->center(player);
}

void load_items(FileReader &file, Entity *player) {
	PlayerComponent *player_component = GetPlayer(player);

	if (file.exists(FILE_PLAYER_INVENTORY)) {
		std::istringstream inventory(file.get_string(FILE_PLAYER_INVENTORY));
		int item_id = 0;
		while (inventory >> item_id) {
			Entity *item = new Entity(TYPE_ITEM, item_id);
			player_component->items.push_back(item);
		}
	}

	if (file.exists(FILE_PLAYER_EQUIPPED)) {
		std::istringstream equipped(file.get_string(FILE_PLAYER_EQUIPPED));
		int item_id = 0;
		int slot = 0;
		while (equipped >> item_id) {
			if (item_id != -1) {
				Entity *item = new Entity(TYPE_ITEM, item_id);
				player_component->equipped_items[slot] = item;
				player_component->equip_item_stats(GetItem(item));
			}
			++slot;
		}
	}
}

void load_info(FileReader &file, Entity *player) {
	PlayerComponent *player_component = GetPlayer(player);
	CombatComponent *stats = GetCombat(player);

	std::string name = "";
	int level = 1, exp = 0;
	int health = 0, mana = 0;

	if (file.exists(FILE_PLAYER_NAME)) name = file.get_string(FILE_PLAYER_NAME);
	if (file.exists(FILE_PLAYER_LEVEL)) level = file.get_int(FILE_PLAYER_LEVEL);
	if (file.exists(FILE_PLAYER_EXP)) exp = file.get_int(FILE_PLAYER_EXP);
	if (file.exists(FILE_PLAYER_HEALTH)) health = file.get_int(FILE_PLAYER_HEALTH);
	if (file.exists(FILE_PLAYER_MANA)) mana = file.get_int(FILE_PLAYER_MANA);

	player_component->name = name;
	player_component->exp = exp;
	stats->health = health;
	stats->mana = mana;

	player_component->level = 1;
	for (int i = 0; i != level - 1; ++i) {
		player_component->level_up();
	}
}

void load_player() {
	FileReader file(PLAYER_FILE);
	Entity *player = Environment::get().get_resource_manager()->get_player();

	load_position(file, player);
	load_items(file, player);
	load_info(file, player);

	if (file.exists(FILE_PLAYER_MAP_ID)) Environment::get().get_resource_manager()->load_map(file.get_int(FILE_PLAYER_MAP_ID), false);
}

void save_player() {
	std::ofstream file;
	file.open(PLAYER_FILE, std::ofstream::out | std::ofstream::trunc);
	Entity *player = Environment::get().get_resource_manager()->get_player();

	file << "# Player" << std::endl;

	CombatComponent *stats = GetCombat(player);
	PlayerComponent *player_component = GetPlayer(player);
	file << FILE_PLAYER_NAME << " " << player_component->name << std::endl;
	file << FILE_PLAYER_LEVEL << " " << player_component->level << std::endl;
	file << FILE_PLAYER_EXP << " " << player_component->exp << std::endl;
	file << FILE_PLAYER_HEALTH << " " << stats->health << std::endl;
	file << FILE_PLAYER_MANA << " " << stats->mana << std::endl;

	PositionComponent *position = GetPosition(player);
	file << FILE_PLAYER_POSITION << " " << position->pos_x << " " << position->pos_y << std::endl;

	file << FILE_PLAYER_INVENTORY << " ";
	for (auto &item : player_component->items) {
		file << item->get_type_id() << " ";
	}
	file << std::endl;

	file << FILE_PLAYER_EQUIPPED << " ";
	for (auto &item : player_component->equipped_items) {
		if (item == nullptr)
			file << -1 << " ";
		else
			file << item->get_type_id() << " ";
	}
	file << std::endl;

	file << FILE_PLAYER_MAP_ID << " " << Environment::get().get_resource_manager()->get_map()->get_id() << std::endl;

	file.close();
}