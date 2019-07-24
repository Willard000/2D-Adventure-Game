#include <string>

#include "Component.h"

#include "Timer.h"

#ifndef Effect_COMPONENT_H
#define Effect_COMPONENT_H

struct EffectComponent : public Component {
	EffectComponent(Entity *entity_, std::string name, std::string script_, int rand_time_offset_range_);

	virtual void update();

	std::string name;
	std::string script;
	int rand_time_offset_range;
};

#endif