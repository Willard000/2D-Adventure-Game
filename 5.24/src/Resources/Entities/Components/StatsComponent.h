#include "Component.h"

#ifndef STATS_COMPONENT_H
#define STATS_COMPONENT_H

struct StatsComponent : public Component {
	StatsComponent(Entity *entity_, int max_health_, int max_mana_, int damage_, int armor_);
	StatsComponent(Entity *new_entity, const StatsComponent &rhs);
	StatsComponent *copy(Entity *new_entity) const;

	void update();

	const int get_type() const ;

	void apply_damage(int damage_);

	int health;
	int max_health;
	int mana;
	int max_mana;

	int damage;
	int armor;
};

#endif