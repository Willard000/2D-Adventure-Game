#include "PlayerComponent.h"

#include "Environment.h"
#include "ResourceManager.h"

#include "PositionComponent.h"

#include "Collision.h"

#include "Text_Timed.h"

#define EXP_TEXT_COLOR {150, 100, 200, 200}
#define EXP_TEXT_FTSIZE 24
#define EXP_TEXT_WRAP_LENGTH 1000
#define EXP_TEXT_DISPLAY_TIME 800

PlayerComponent::PlayerComponent(Entity *entity_, std::string name_) :
	Component		 ( entity_ ),
	name			 ( name_ ),
	level			 ( 0 ),
	exp				 ( 0 ),
	exp_to_level     ( 100 ),
	inventory		 ( entity_, &items, GetCombat(entity_) ),
	debug_collision  ( false )
{}

PlayerComponent::PlayerComponent(Entity *new_entity, const PlayerComponent &rhs) :
	Component		 ( new_entity ),
	name			 ( rhs.name ),
	level			 ( rhs.level ),
	exp				 ( rhs.exp ),
	exp_to_level	 ( rhs.exp_to_level),
	items			 ( rhs.items ),
	inventory		 ( new_entity, &items, GetCombat(new_entity) ),
	debug_collision  ( rhs.debug_collision )
{}

PlayerComponent *PlayerComponent::copy(Entity *new_entity) const {
	return new PlayerComponent(new_entity, *this);
}

PlayerComponent::~PlayerComponent() {
	for (auto &item : items) {
		delete item;
	}

	for (auto &item : equipped_items) {
		delete item;
	}
}

void PlayerComponent::update() {
	// WARP
	if (PositionComponent *position = GetPosition(entity)) {
		Map::Warp *map_warp = Environment::get().get_resource_manager()->get_map()->warp_collision(position->rect);
		if (map_warp != nullptr) {
			warp(map_warp->to_id, map_warp->to);
		}
	}

	// exp
	if (exp >= exp_to_level) {
		level_up();
		exp = 0;
	}
}

bool PlayerComponent::is_collision() {
	if (debug_collision)
		return false;

	PositionComponent *position = GetPosition(entity);
	if (!position)
		return false;

	const auto entity_vec = Environment::get().get_resource_manager()->get_map()->get_entity_grid()->get_cells(position->rect);

	for (auto &vec : entity_vec) {
		for (auto &e : *vec) {
			const int &type = e->get_type();
			if (type == TYPE_OBJECT ||
				type == TYPE_ENEMY) {
				if (collision(position->rect, GetPosition(e)->rect))
					return true;
			}
		}
	}

	if (Environment::get().get_resource_manager()->get_map()->solid_collision(position->rect) ||
		Environment::get().get_resource_manager()->get_map()->bound_collision(position->rect))
		return true;

	return false;
}

void PlayerComponent::equip_item(int index) {
	if (index < 0 || index > (int)items.size()) {
		return;
	}

	ItemComponent *item_info = GetItem(items[index]);

	if (equipped_items[item_info->slot]) {
		items.push_back(equipped_items[item_info->slot]);
		unequip_item_stats(GetItem(equipped_items[item_info->slot]));
	}
	equipped_items[item_info->slot] = items[index];
	equip_item_stats(item_info);
	items.erase(items.begin() + index);
}

void PlayerComponent::unequip_item(int index) {
	if (index < 0 || index > TOTAL_SLOTS - 1) {
		return;
	}

	if (items.size() == MAX_ITEMS) {
		return;
	}

	if (!equipped_items[index]) {
		return;
	}

	ItemComponent *item_info = GetItem(equipped_items[index]);
	unequip_item_stats(item_info);
	items.push_back(equipped_items[index]);
	equipped_items[index] = nullptr;
}

void PlayerComponent::equip_item_stats(ItemComponent *item) {
	CombatComponent *stats = GetCombat(entity);
	PositionComponent *position = GetPosition(entity);

	stats->max_health += item->health;
	stats->max_mana += item->mana;
	stats->damage += item->damage;
	stats->armor += item->armor;
	stats->hps += item->hps;
	stats->mps += item->mps;
	stats->drain += item->drain;
	position->speed += item->speed;
	stats->luck += item->luck;
}

void PlayerComponent::unequip_item_stats(ItemComponent *item) {
	CombatComponent *stats = GetCombat(entity);
	PositionComponent *position = GetPosition(entity);

	stats->max_health -= item->health;
	if (stats->health > stats->max_health)
		stats->health = stats->max_health;
	stats->max_mana -= item->mana;
	if (stats->mana > stats->max_mana)
		stats->mana = stats->max_mana;
	stats->damage -= item->damage;
	stats->armor -= item->armor;
	stats->hps -= item->hps;
	stats->mps -= item->mps;
	stats->drain -= item->drain;
	position->speed -= item->speed;
	stats->luck -= item->luck;
}

void PlayerComponent::level_up() {
	++level;
	exp_to_level = int(exp_to_level * 1.1f + 50); // change this?

	PositionComponent *position = GetPosition(entity);
	CombatComponent *stats = GetCombat(entity);
	stats->max_health += 5 * level;
	stats->health = stats->max_health;
	stats->max_mana += 5 * level;
	stats->mana = stats->max_mana;
	stats->damage += 1;
	stats->armor += 1;
	if (level % 5 == 0) {
		stats->hps += 2;
		stats->mps += 2;
		position->speed += 10;
		stats->luck += 1;
	}
}

void PlayerComponent::add_exp(int amount) {
	exp += amount;

	PositionComponent *position = GetPosition(entity);
	int x = position->rect.x + rand() % position->rect.w;
	int y = position->rect.y - rand() % position->rect.h;

	Text_Timed *exp_text = new Text_Timed(
		std::to_string(amount),
		EXP_TEXT_COLOR,
		EXP_TEXT_FTSIZE,
		EXP_TEXT_WRAP_LENGTH,
		x,
		y,
		EXP_TEXT_DISPLAY_TIME
	);

	Environment::get().get_resource_manager()->add_text(exp_text);
}

void PlayerComponent::warp(int map_id, SDL_Rect warp_rect) {
	PositionComponent *position = GetPosition(entity);
	position->set(float(warp_rect.x + (warp_rect.w / 2) - (position->rect.w / 2)), float(warp_rect.y + (warp_rect.h / 2) - (position->rect.h / 2)));
	Environment::get().get_resource_manager()->get_map()->save(false);
	Environment::get().get_resource_manager()->load_map(map_id, false);
}