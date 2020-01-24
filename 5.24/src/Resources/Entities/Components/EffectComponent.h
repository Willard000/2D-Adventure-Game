#include <string>

#include "Component.h"

#include "Timer.h"

#ifndef Effect_COMPONENT_H
#define Effect_COMPONENT_H

struct EffectComponent : public Component {
	EffectComponent(Entity *entity_, std::string name, std::string script_, int rand_rotation_, int rand_time_, int duration_);
	EffectComponent(Entity *new_entity, const EffectComponent &rhs);
	EffectComponent *copy(Entity *new_entity) const;

	void update();

	const int get_type() const { return COMPONENT_EFFECT; }

	std::string name;
	std::string script;
	int rand_rotation;
	int rand_time;
	Timer duration;
};

#endif