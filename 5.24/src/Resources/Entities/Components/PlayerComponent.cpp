#include "PlayerComponent.h"

#include "Environment.h"
#include "ResourceManager.h"

#include "PositionComponent.h"

#include "Collision.h"

PlayerComponent::PlayerComponent(Entity *entity_, std::string name_) :
	Component		 ( entity_ ),
	name			 ( name_ ),
	inventory		 ( entity_, &items, GetCombat(entity_) )
{}

PlayerComponent::PlayerComponent(Entity *new_entity, const PlayerComponent &rhs) :
	Component		 ( new_entity ),
	name			 ( rhs.name ),
	items			 ( rhs.items ),
	inventory		 ( new_entity, &items, GetCombat(new_entity) )
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
		Map::Warp *warp = Environment::get().get_resource_manager()->get_map()->warp_collision(position->rect);
		if (warp != nullptr) {
			position->set(float(warp->to.x + (warp->to.w / 2) - (position->rect.w / 2)), float(warp->to.y + (warp->to.h / 2) - (position->rect.h / 2)));
			if (warp->to_id != Environment::get().get_resource_manager()->get_map()->get_id())
				Environment::get().get_resource_manager()->load_map(warp->to_id);
		}
	}
}

bool PlayerComponent::is_collision() {
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
	if (index < 0 || index > items.size()) {
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
	if (index < 0 || index > items.size()) {
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

	stats->max_health += item->health;
	stats->max_mana += item->mana;
	stats->damage += item->damage;
	stats->armor += item->armor;
	stats->hps += item->hps;
	stats->mps += item->mps;
}

void PlayerComponent::unequip_item_stats(ItemComponent *item) {
	CombatComponent *stats = GetCombat(entity);

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
}