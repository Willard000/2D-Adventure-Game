#include <vector>
#include <array>
#include <string>

#include "Component.h"
#include "Entity.h"

#include "Inventory.h"

#ifndef PLAYER_COMPONENT_H
#define PLAYER_COMPONENT_H

#define MAX_ITEMS 100

struct PlayerComponent : public Component {
	PlayerComponent(Entity *entity_, std::string name);
	PlayerComponent(Entity *new_entity, const PlayerComponent &rhs);
	PlayerComponent *copy(Entity *new_entity) const;

	~PlayerComponent();

	void update();

	const int get_type() const { return COMPONENT_PLAYER; }

	bool is_collision();

	void equip_item(int index);
	void unequip_item(int index);
	void equip_item_stats(ItemComponent *item);
	void unequip_item_stats(ItemComponent *item);

	void level_up();

	int level, exp, exp_to_level;
	std::string name;
	std::vector<Entity *> items;
	std::array<Entity *, TOTAL_SLOTS> equipped_items = { nullptr };

	Inventory inventory;
};

#endif