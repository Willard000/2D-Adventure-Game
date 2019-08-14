#include <string>

#include "Component.h"

#ifndef ITEM_COMPONENT_H
#define ITEM_COMPONENT_H

struct ItemComponent : public Component {
	ItemComponent(Entity *entity_, std::string name_, int health_, int mana_, int damage_, int armor_, int hps_, int mps_, bool is_equipable_, bool is_useable_);
	ItemComponent(Entity *new_entity, const ItemComponent &rhs);
	ItemComponent *copy(Entity *new_entity) const;

	void update() {};

	const int get_type() const { return COMPONENT_ITEM; }

	std::string name;

	int health, mana;
	int damage, armor;
	int hps, mps;

	bool is_equipable;
	bool is_useable;
};

#endif