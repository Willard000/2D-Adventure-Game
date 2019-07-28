#include "Component.h"

#include <string>
#include <SDL_rect.h>

#ifndef ENEMY_COMPONENT_H
#define ENEMY_COMPONENT_H

struct Combat_Range {
	int w, h;
};

struct EnemyComponent : public Component {
	EnemyComponent(Entity *entity_, std::string name_, std::string script_, Combat_Range combat_range);
	EnemyComponent(Entity *new_entity, const EnemyComponent &rhs);
	EnemyComponent *copy(Entity *new_entity) const;

	void update();

	const int get_type() const { return COMPONENT_ENEMY; }

	bool is_collision();

	bool check_combat_range();
	bool in_combat;

	std::string name;
	std::string script;

	Combat_Range combat_range;
};

#endif