#include <vector>
#include <string>

#include "Component.h"
#include "Entity.h"

#ifndef PLAYER_COMPONENT_H
#define PLAYER_COMPONENT_H

struct PlayerComponent : public Component {
	PlayerComponent(Entity *entity_, std::string name);
	PlayerComponent(Entity *new_entity, const PlayerComponent &rhs);
	PlayerComponent *copy(Entity *new_entity) const;

	~PlayerComponent();

	void update();

	const int get_type() const { return COMPONENT_PLAYER; }

	bool is_collision();

	std::string name;
	std::vector<Entity *> items;
};

#endif