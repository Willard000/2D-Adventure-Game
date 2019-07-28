#include "Component.h"

#include "_Lua.h"

#include <SDL.h>

#include <string>

#include "Timer.h"

#ifndef SPELL_COMPONENT_H
#define SPELL_COMPONENT_H

struct SpellComponent : public Component {
	SpellComponent(Entity *entity_, std::string name_, float max_dis_, float speed_, int death_time, std::string script_, int damage_);
	SpellComponent(Entity *new_entity, const SpellComponent &rhs);
	SpellComponent *copy(Entity *new_entity) const;

	void update();

	const int get_type() const { return COMPONENT_SPELL; }

	void cast(float x, float y);

	void move(float x, float y);

	bool is_collision();

	void death();

	int calc_damage();

	Entity *caster;
	float dis, max_dis;
	float speed;
	float dx, dy; // vec2 direction
	bool dead;
	bool destroy;
	Timer death_timer;

	int damage;

	std::string name;
	std::string script;
};

#endif
