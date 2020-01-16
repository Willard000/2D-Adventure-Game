#include "Component.h"
#include "CombatComponent.h"

#include "_Lua.h"

#include <SDL.h>

#include <string>

#include "Timer.h"

#ifndef SPELL_COMPONENT_H
#define SPELL_COMPONENT_H

enum SPELL_TYPE {
	FIRE,
	WATER,
	EARTH,
	AIR,
	OTHER
};

struct SpellComponent : public Component {
	SpellComponent(Entity *entity_, float max_dis_, float speed_, int death_time, std::string script_name_, std::string script_, int damage_, int mana_cost_, int spell_type_, SDL_Color color_);
	SpellComponent(Entity *new_entity, const SpellComponent &rhs);
	SpellComponent *copy(Entity *new_entity) const;

	void update();

	const int get_type() const { return COMPONENT_SPELL; }

	void cast(float x, float y, float x2, float y2, Combat_Info attacker_info_);

	void move(float x, float y);

	bool is_collision();
	void on_collision(Entity *e);

	void death();

	Entity *caster;
	float dis, max_dis;
	float speed;
	float dx, dy; // vec2 direction
	bool dead;
	bool destroy;
	Timer death_timer;
	SDL_Color color;

	int damage;
	int mana_cost;

	int spell_type;

	bool caster_deleted;

	std::string script_name;
	std::string script;

	Combat_Info attacker_info;
};

#endif
