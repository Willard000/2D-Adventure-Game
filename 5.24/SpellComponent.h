#include "Component.h"

#include <SDL.h>

#include <string>

#ifndef SPELL_COMPONENT_H
#define SPELL_COMPONENT_H

struct SpellComponent : public Component {
	SpellComponent(Entity *entity_, double max_dis_, double speed_, int death_time, std::string script_);

	virtual void update();

	void cast();

	void death();

	Entity *owner;
	double dis, max_dis;
	double speed;
	double dx, dy;
	double des_x, des_y;
	bool dead;
	bool destroy;
	int death_time;

	std::string script;
};

#endif
