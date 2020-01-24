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

#define LEVEL_COLOR {150, 100, 200, 200}
#define EXP_COLOR {150, 100, 200, 200}
#define HEALTH_COLOR {225, 55, 55, 200}
#define MANA_COLOR {100, 185, 225, 200}
#define DAMAGE_COLOR {240, 160, 110, 200}
#define ARMOR_COLOR {210, 130, 200, 200}
#define HPS_COLOR {225, 55, 55, 200}
#define MPS_COLOR {100, 185, 225, 200}
#define DRAIN_COLOR {100, 185, 225, 200}
#define SPEED_COLOR {135, 210, 140, 200}
#define LUCK_COLOR {250, 170, 100, 200}
#define DURATION_COLOR {200, 200, 200, 200}
#define VALUE_COLOR { 225, 185, 25, 255 }
#define STACK_SIZE_COLOR {255, 255, 255, 255}

#define TOTAL_PLAYER_SPELLS 16

class Inventory {
public:
	struct Item_Info {
		Text name, info, health, mana, damage, armor, hps, mps, drain, speed, luck, duration;
		Text health_val, mana_val, damage_val, armor_val, hps_val, mps_val, drain_val, speed_val, luck_val, duration_val, sell_value_val, buy_value_val, stack_size_val;
		bool hide;
	};

	struct Entity_Info {
		Text name, level, exp, health, mana, damage, armor, mps, hps, drain, speed, luck, gold;
		Text level_val, exp_val, health_val, mana_val, damage_val, armor_val, mps_val, hps_val, drain_val, speed_val, luck_val, gold_val;
	};

	struct Selection {
		bool equipped;
		int index;
	};

	struct Spell_Info {
		SDL_Rect rect;
		int icon_id;
		int id;
	};
public:
	Inventory(Entity *entity, std::vector<Entity *> *items, CombatComponent *stats);
	~Inventory();

	void open();
	void equip_item();
	void unequip_item();
	void use_item();
	void drop_item();
	void add_item(int item_id);
	void pause_buffs();
	void select_spell(Spell_Info &spell, int index, bool main_spell = true);
private:
	void input();
	void update();
	void render();

	void render_entity();
	void render_slots();
	void render_spells();
	void render_items();
	void render_item_info();
	void render_entity_info();
	void render_buttons();

	void select_item(int mouse_x, int mouse_y);
	void set_item_info(ItemComponent *item);

	void set_entity_info();

	void set_item_equipped(bool is_equipped);

	void update_inventory_size();
	void update_selection_info();
	void update_spell_selection();

	void setup_buttons();
	void setup_slots();
	void setup_spells();
	
	bool _exit;

	SDL_Rect _background;
	SDL_Rect _items_background;
	SDL_Rect _spell_background;
	SDL_Rect _main_spell_selection;
	SDL_Rect _secondary_spell_selection;
	Entity *_entity;
	std::vector<Entity *> *_items;
	CombatComponent *_stats;

	Selection _selection;
	Item_Info _selection_info;
	Entity_Info _entity_info;
	Text _inventory_size;

	std::vector<UI::Button *> _buttons;
	std::array<SDL_Rect, TOTAL_SLOTS> _slots = { {0, 0, 0, 0} };
	std::array<Spell_Info, TOTAL_PLAYER_SPELLS> _spells = { {0, 0, 0, 0} };
};

#endif