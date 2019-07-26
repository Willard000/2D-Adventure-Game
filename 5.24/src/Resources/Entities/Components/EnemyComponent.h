#include "Component.h"

#include <string>

#ifndef ENEMY_COMPONENT_H
#define ENEMY_COMPONENT_H

struct EnemyComponent : public Component {
	EnemyComponent(Entity *entity_, std::string name_, std::string script_);
	EnemyComponent(Entity *new_entity, const EnemyComponent &rhs);
	EnemyComponent *copy(Entity *new_entity) const;

	void update();

	const int get_type() const { return COMPONENT_ENEMY; }

	bool is_collision();

	std::string name;
	std::string script;
};

#endif