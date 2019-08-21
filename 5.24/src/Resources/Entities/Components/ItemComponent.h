#include <string>

#include "Component.h"

#ifndef ITEM_COMPONENT_H
#define ITEM_COMPONENT_H

enum {
	SLOT_WEAPON,
	SLOT_OFFHAND,
	SLOT_HEAD,
	SLOT_CHEST,
	SLOT_LEGS,
	SLOT_GLOVES,
	SLOT_BOOTS,
	SLOT_RING,

	TOTAL_SLOTS
};

struct ItemComponent : public Component {
	ItemComponent(
		Entity *entity_,
		int slot_,
		std::string name_,
		int health_,
		int mana_,
		int damage_,
		int armor_,
		int hps_,
		int mps_,
		int drain_,
		int speed_,
		int luck_,
		bool is_equipable_,
		bool is_useable_
	);
	ItemComponent(Entity *new_entity, const ItemComponent &rhs);
	ItemComponent *copy(Entity *new_entity) const;

	void update() {};

	const int get_type() const { return COMPONENT_ITEM; }

	int slot;

	std::string name;

	int health, mana;
	int damage, armor;
	int hps, mps;
	int drain;
	int speed, luck;

	bool is_equipable;
	bool is_useable;
};

#endif