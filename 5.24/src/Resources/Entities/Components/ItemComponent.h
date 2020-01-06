#include <string>

#include "Component.h"

#include <SDL.h>

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
		std::string info_,
		int health_,
		int mana_,
		int damage_,
		int armor_,
		int hps_,
		int mps_,
		int drain_,
		int speed_,
		int luck_,
		int duration_,
		bool is_equipable_,
		bool is_useable_,
		bool is_buffable_,
		bool is_stackable_,
		std::string script_name_,
		std::string script_,
		SDL_Color color_
	);
	ItemComponent(Entity *new_entity, const ItemComponent &rhs);
	ItemComponent *copy(Entity *new_entity) const;

	void update() {};

	const int get_type() const { return COMPONENT_ITEM; }

	void use(Entity *owner);

	int slot;

	std::string name;
	std::string info;

	std::string script_name;
	std::string script;

	SDL_Color color;

	int health, mana;
	int damage, armor;
	int hps, mps;
	int drain;
	int speed, luck;
	int duration;

	int stack_size;

	bool is_equipable;
	bool is_useable;
	bool is_buffable;
	bool is_stackable;
	bool destroy;
};

#endif