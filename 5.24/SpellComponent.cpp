#include "SpellComponent.h"

#include "Entity.h"
#include "PositionComponent.h"
#include "SpriteComponent.h"

#include "Environment.h"
#include "ScriptManager.h"
#include "ResourceManager.h"

#include "lua5.1/luawrapper.hpp"

#include "Timer.h"

#define LUA_SPELL_CASTED "SPELL_CASTED"

SpellComponent::SpellComponent(Entity *entity_, double max_dis_, double speed_, int death_time_, std::string script_) :
	Component			( entity_ ),
	owner				( nullptr ),
	dis					( 0 ),
	max_dis				( max_dis_ ),
	speed				( speed_ ),
	dx					( 0 ),
	dy					( 0 ),
	des_x				( 0 ),
	des_y				( 0 ),
	dead				( false ),
	destroy				( false ),
	death_time			( death_time_ ),
	script				( script_ )
{}

void SpellComponent::update() {
	static lua_State *thread = lua_newthread(Environment::get().get_script_manager()->getL());
	static bool loaded = false;
	if (!loaded) {
		luaL_dofile(thread, script.c_str());
		loaded = true;
	}


	if (!dead) {
		lua_getglobal(thread, "update");
		luaW_push<Entity>(thread, entity);
		lua_resume(thread, 1);
	}
	else {
		static Timer death_timer(death_time);
		if (death_timer.update()) {
			entity->destroy();
		}

	}

}

void SpellComponent::cast() {
	lua_State *L = Environment::get().get_script_manager()->getL();

	luaW_push<Entity>(L, entity);
	lua_setglobal(L, LUA_SPELL_CASTED);

	luaL_dofile(L, script.c_str());
}

void SpellComponent::death() {
	dead = true;
	if (SpriteComponent *sprite = GetSprite(entity)) {
		Sprite *sprite_info = Environment::get().get_resource_manager()->get_sprite_info(entity);
		sprite->pos.x = sprite_info->get_frame(sprite_info->end).x;
		sprite->pos.y = sprite_info->get_frame(sprite_info->end).y;
	}
}