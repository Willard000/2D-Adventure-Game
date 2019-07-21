#include "Component.h"

#ifndef ENEMY_COMPONENT_H
#define ENEMY_COMPONENT_H

struct EnemyComponent : public Component {
	EnemyComponent(Entity *entity_, std::string name_, std::string script_);

	virtual void update();

	std::string name;
	std::string script;
};

#endif