#include <SDL_rect.h>
#include <vector>

#include "Entity.h"
#include "CombatComponent.h"
#include "ItemComponent.h"

#include "Text.h"

#include "Button.h"
#include "Button_Pressable.h"

#ifndef INVENTORY_H
#define INVENTORY_H

class Inventory {
public:
	struct Item_Info {
		Text name, health, mana, damage, armor, hps, mps, leech, drain, speed, luck;
		Text health_val, mana_val, damage_val, armor_val, hps_val, mps_val, leech_val, drain_val, speed_val, luck_val;
		bool hide;
	};

	struct Entity_Info {
		Text name, level, exp, health, mana, damage, armor, mps, hps, leech, drain, speed, luck;
		Text level_val, exp_val, health_val, mana_val, damage_val, armor_val, mps_val, hps_val, leech_val, drain_val, speed_val, luck_val;
	};

	struct Selection {
		bool equipped;
		int index;
	};
public:
	Inventory(Entity *entity, std::vector<Entity *> *items, CombatComponent *stats);
	~Inventory();

	void open();
	void equip_item();
	void unequip_item();
	void use_item();
	void drop_item();
private:
	void input();
	void update();
	void render();

	void render_entity();
	void render_slots();
	void render_items();
	void render_item_info();
	void render_entity_info();
	void render_buttons();

	void select_item(int mouse_x, int mouse_y);
	void set_item_info(ItemComponent *item);

	void set_entity_info();

	void set_item_equipped(bool is_equipped);

	void update_inventory_size();

	void setup_buttons();
	void setup_slots();
	
	bool _exit;

	SDL_Rect _background;
	SDL_Rect _items_background;
	Entity *_entity;
	std::vector<Entity *> *_items;
	CombatComponent *_stats;

	Selection _selection;
	Item_Info _selection_info;
	Entity_Info _entity_info;
	Text _inventory_size;

	std::vector<UI::Button *> _buttons;
	std::array<SDL_Rect, TOTAL_SLOTS> _slots = { {0, 0, 0, 0} };
};

#endif