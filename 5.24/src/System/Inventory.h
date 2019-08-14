#include <SDL_rect.h>
#include <vector>

#include "Entity.h"
#include "CombatComponent.h"
#include "ItemComponent.h"

#include "Text.h"

#ifndef INVENTORY_H
#define INVENTORY_H

class Inventory {
public:
	struct Item_Info {
		Text name, health, mana, damage, armor, hps, mps;
		Text health_val, mana_val, damage_val, armor_val, hps_val, mps_val;
	};

	struct Entity_Info {
		Text name, health, mana, damage, armor, mps, hps;
		Text health_val, mana_val, damage_val, armor_val, mps_val, hps_val;
	};
public:
	Inventory(Entity *entity, std::vector<Entity *> *items, CombatComponent *stats);
	void open();
private:
	void input();
	void update();
	void render();

	void render_entity();
	void render_items();
	void render_item_info();
	void render_entity_info();

	void select_item(int mouse_x, int mouse_y);
	void set_item_info(ItemComponent *item);

	void set_entity_info();

	void update_inventory_size();
	
	bool _exit;

	SDL_Rect _background;
	SDL_Rect _items_background;
	Entity *_entity;
	std::vector<Entity *> *_items;
	CombatComponent *_stats;

	int _selection;
	Item_Info _selection_info;
	Entity_Info _entity_info;
	Text _inventory_size;
};

#endif