#include <vector>

#include "Component.h"
#include "Entity.h"

#include "Timer.h"

#ifndef MAGIC_COMPONENT_H
#define MAGIC_COMPONENT_H

struct MagicComponent : public Component {
	MagicComponent(Entity *entity_, int main_spell_id_, int cast_speed_);

	virtual void update();

	void cast_main(double x_, double y_);

	Timer cast_timer;
	bool can_cast;

	Entity main_spell;
	std::vector<Entity> spells;
};

#endif