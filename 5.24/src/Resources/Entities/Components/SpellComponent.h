#include "Component.h"

#include "_Lua.h"

#include <SDL.h>

#include <string>

#include "Timer.h"

#ifndef SPELL_COMPONENT_H
#define SPELL_COMPONENT_H

struct SpellComponent : public Component {
	SpellComponent(Entity *entity_, float max_dis_, float speed_, int death_time, std::string script_);

	virtual void update();

	void cast();

	void death();

	Entity *owner;
	float dis, max_dis;
	float speed;
	float dx, dy; // vec2 direction
	bool dead;
	bool destroy;
	Timer death_timer;
	lua_State *lua_thread;

	std::string script;
};

#endif
