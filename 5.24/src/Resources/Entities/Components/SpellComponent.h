#include "Component.h"

#include "_Lua.h"

#include <SDL.h>

#include <string>

#include "Timer.h"

#ifndef SPELL_COMPONENT_H
#define SPELL_COMPONENT_H

struct SpellComponent : public Component {
	SpellComponent(Entity *entity_, std::string name_, float max_dis_, float speed_, int death_time, std::string script_);

	virtual void update();

	void cast();

	void move(float x, float y);

	bool is_collision();

	void death();

	Entity *caster;
	float dis, max_dis;
	float speed;
	float dx, dy; // vec2 direction
	bool dead;
	bool destroy;
	Timer death_timer;

	std::string name;
	std::string script;
};

#endif
