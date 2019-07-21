#include "SpellComponent.h"

#include "Entity.h"
#include "PositionComponent.h"
#include "SpriteComponent.h"

#include "Environment.h"
#include "_Lua.h"
#include "ResourceManager.h"

#include "luawrapper.hpp"

#include <iostream>

SpellComponent::SpellComponent(Entity *entity_, std::string name_, float max_dis_, float speed_, int death_time_, std::string script_) :
	Component			( entity_ ),
	name				( name_ ),
	caster				( nullptr ),
	dis					( 0 ),
	max_dis				( max_dis_ ),
	speed				( speed_ ),
	dx					( 0 ),
	dy					( 0 ),
	dead				( false ),
	destroy				( false ),
	death_timer			( death_time_ ),
	script				( script_ )
{
	Environment::get().get_lua()->load_script(script);
}

void SpellComponent::update() {
	if (!dead) {
		lua_State *L = Environment::get().get_lua()->get_state();
		lua_getglobal(L, name.c_str());
		lua_getfield(L, -1, "update");
		lua_remove(L, -2);
		luaW_push<SpellComponent>(L, this);
		lua_pcall(L, 1, 0, 0);
	}
	else {
		if (death_timer.update()) {
			entity->destroy();
		}
	}
}

void SpellComponent::cast() {
	
	lua_State *L = Environment::get().get_lua()->get_state();
	lua_getglobal(L, name.c_str());
	lua_getfield(L, -1, "cast");
	lua_remove(L, -2);
	luaW_push<SpellComponent>(L, this);
	lua_pcall(L, 1, 0, 0);
}

void SpellComponent::death() {
	dead = true;
	death_timer.reset();
	if (SpriteComponent *sprite = GetSprite(entity)) {
		Sprite *sprite_info = Environment::get().get_resource_manager()->get_sprite_info(entity);
		sprite->pos.x = sprite_info->get_frame(sprite_info->end).x;
		sprite->pos.y = sprite_info->get_frame(sprite_info->end).y;
		sprite->time = -1;
	}
}