#include "Component.h"

#ifndef PLAYER_COMPONENT_H
#define PLAYER_COMPONENT_H

struct PlayerComponent : public Component {
	PlayerComponent(Entity *entity_);
	PlayerComponent(Entity *new_entity, const PlayerComponent &rhs);
	PlayerComponent *copy(Entity *new_entity) const;

	void update();

	const int get_type() const { return COMPONENT_PLAYER; }

	bool is_collision();
};

#endif