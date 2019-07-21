#include "Component.h"

#ifndef PLAYER_COMPONENT_H
#define PLAYER_COMPONENT_H

struct PlayerComponent : public Component {
	PlayerComponent(Entity *entity_);

	virtual void update();
};

#endif