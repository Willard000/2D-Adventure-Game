#include "EffectComponent.h"

#include "Environment.h"
#include "_Lua.h"

#include "Entity.h"
#include "SpriteComponent.h"
#include "PositionComponent.h"

#include <cstdlib>

#include <iostream>

EffectComponent::EffectComponent(Entity *entity_, std::string name_, std::string script_, int rand_rotation_, int rand_time_, int duration_) :
	Component				( entity_ ),
	name					( name_ ),
	script					( script_ ),
	rand_rotation			( rand_rotation_ ),
	rand_time				( rand_time_ ),
	duration				( duration_ )
{
	Environment::get().get_lua()->load_script(script);

	if (PositionComponent *position = GetPosition(entity)) {
		if(rand_rotation != 0) 
			position->set_rotation((rand() % rand_rotation) % 360);
	}

	if (SpriteComponent *sprite = GetSprite(entity)) {
		if (rand_time > 0) {
			int offset = rand() % 2 == 1 ? sprite->time.get_time() + (rand() % rand_time) : sprite->time.get_time() - (rand() % rand_time);
			sprite->time.offset(offset);
		}
	}

}

EffectComponent::EffectComponent(Entity *new_entity, const EffectComponent &rhs) :
	Component				 ( new_entity ),
	name					 ( rhs.name ),
	script					 ( rhs.script ),
	rand_rotation		     ( rhs.rand_rotation ),
	rand_time				 ( rhs.rand_time),
	duration				 ( rhs.duration )
{
	Environment::get().get_lua()->load_script(script);

	if (PositionComponent *position = GetPosition(entity)) {
		position->set_rotation((rand() % rand_rotation) % 360);
	}

	if (SpriteComponent *sprite = GetSprite(entity)) {
		if (rand_time > 0) {
			int offset = rand() % 2 == 1 ? sprite->time.get_time() + (rand() % rand_time) : sprite->time.get_time() - (rand() % rand_time);
			sprite->time.offset(offset);
		}
	}
}

EffectComponent *EffectComponent::copy(Entity *new_entity) const {
	return new EffectComponent(new_entity, *this);
}

void EffectComponent::update() {
	lua_State *L = Environment::get().get_lua()->get_state();
	lua_getglobal(L, name.c_str());
	lua_getfield(L, -1, "update");
	lua_remove(L, -2);
	luaW_push<EffectComponent>(L, this);
	lua_pcall(L, 1, 0, 0);

	if (duration.update()) {
		entity->destroy();
	}
}

