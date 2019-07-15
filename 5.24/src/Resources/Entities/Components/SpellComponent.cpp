#include "SpellComponent.h"

#include "Entity.h"
#include "PositionComponent.h"
#include "SpriteComponent.h"

#include "Environment.h"
#include "_Lua.h"
#include "ResourceManager.h"

#include "luawrapper.hpp"

#define LUA_SPELL_CASTED "SPELL_CASTED"

SpellComponent::SpellComponent(Entity *entity_, float max_dis_, float speed_, int death_time_, std::string script_) :
	Component			( entity_ ),
	owner				( nullptr ),
	dis					( 0 ),
	max_dis				( max_dis_ ),
	speed				( speed_ ),
	dx					( 0 ),
	dy					( 0 ),
	dead				( false ),
	destroy				( false ),
	death_timer			( death_time_ ),
	script				( script_ ),
	lua_thread			( lua_newthread(Environment::get().get_lua()->get_state()) )
{
	luaL_dofile(lua_thread, script.c_str());
}

void SpellComponent::update() {
	if (!dead) {
		lua_getglobal(lua_thread, "update");
		luaW_push<Entity>(lua_thread, entity);
		lua_resume(lua_thread, 1);
	}
	else {
		if (death_timer.update()) {
			entity->destroy();
		}
	}

}

void SpellComponent::cast() {
	lua_State *L = Environment::get().get_lua()->get_state();

	luaW_push<Entity>(L, entity);
	lua_setglobal(L, LUA_SPELL_CASTED);

	luaL_dofile(L, script.c_str());
}

void SpellComponent::death() {
	dead = true;
	death_timer.restart();
	if (SpriteComponent *sprite = GetSprite(entity)) {
		Sprite *sprite_info = Environment::get().get_resource_manager()->get_sprite_info(entity);
		sprite->pos.x = sprite_info->get_frame(sprite_info->end).x;
		sprite->pos.y = sprite_info->get_frame(sprite_info->end).y;
	}
}