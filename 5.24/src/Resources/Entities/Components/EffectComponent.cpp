#include "EffectComponent.h"

#include "Environment.h"
#include "_Lua.h"

#include "Entity.h"
#include "SpriteComponent.h"

#include <cstdlib>

#include <iostream>
EffectComponent::EffectComponent(Entity *entity_, std::string name_, std::string script_, int rand_time_offset_range_) :
	Component				( entity_ ),
	name					( name_ ),
	script					( script_ )
{
	Environment::get().get_lua()->load_script(script);

	if (SpriteComponent *sprite = GetSprite(entity)) {
		if (rand_time_offset_range_ > 0) {
			int offset = rand() % 2 == 1 ? sprite->time.get_time() + (rand() % rand_time_offset_range_) : sprite->time.get_time() - (rand() % rand_time_offset_range_);
			sprite->time.set(offset);
		}
	}
}

void EffectComponent::update() {
	lua_State *L = Environment::get().get_lua()->get_state();
	lua_getglobal(L, name.c_str());
	lua_getfield(L, -1, "update");
	lua_remove(L, -2);
	luaW_push<EffectComponent>(L, this);
	lua_pcall(L, 1, 0, 0);
}

