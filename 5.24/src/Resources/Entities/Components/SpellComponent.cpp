#include "SpellComponent.h"

#include "Entity.h"
#include "PositionComponent.h"
#include "SpriteComponent.h"
#include "CombatComponent.h"

#include "Environment.h"
#include "Clock.h"
#include "_Lua.h"
#include "ResourceManager.h"

#include "luawrapper.hpp"

#include <iostream>

SpellComponent::SpellComponent(Entity *entity_, float max_dis_, float speed_, int death_time_, std::string script_name_, std::string script_, int damage_, SDL_Color color_) :
	Component			( entity_ ),
	caster				( nullptr ),
	dis					( 0 ),
	max_dis				( max_dis_ ),
	speed				( speed_ ),
	dx					( 0 ),
	dy					( 0 ),
	dead				( false ),
	destroy				( false ),
	death_timer			( death_time_ ),
	script_name			( script_name_ ),
	script				( script_ ),
	damage				( damage_ ),
	color				( color_ )
{
	Environment::get().get_lua()->load_script(script);
}

SpellComponent::SpellComponent(Entity *new_entity, const SpellComponent &rhs) :
	Component			( new_entity ),
	caster				( rhs.caster ),
	dis					( rhs.dis ),
	max_dis				( rhs.max_dis ),
	speed				( rhs.speed ),
	dx					( rhs.dx ),
	dy					( rhs.dy ),
	dead				( rhs.dead ),
	destroy				( rhs.destroy ),
	death_timer			( rhs.death_timer ),
	script_name			( rhs.script_name ),
	script				( rhs.script ),
	damage				( rhs.damage ),
	color				( rhs.color )
{}

SpellComponent *SpellComponent::copy(Entity *new_entity) const {
	return new SpellComponent(new_entity, *this);
}

void SpellComponent::update() {
	if (!dead) {
		lua_State *L = Environment::get().get_lua()->get_state();
		lua_getglobal(L, script_name.c_str());
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

void SpellComponent::cast(float x, float y) {
	lua_State *L = Environment::get().get_lua()->get_state();
	lua_getglobal(L, script_name.c_str());
	lua_getfield(L, -1, "cast");
	lua_remove(L, -2);
	luaW_push<SpellComponent>(L, this);
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_pcall(L, 3, 0, 0);

	SpriteComponent *sprite = GetSprite(caster);
	if (sprite) {
		sprite->ani = CAST;
	}
}

void SpellComponent::move(float x, float y) {
	float xdis = float(x * Environment::get().get_clock()->get_time());
	float ydis = float(y * Environment::get().get_clock()->get_time());

	dis += abs(xdis) + abs(ydis);

	if (PositionComponent *position = GetPosition(entity)) {
		position->pos_x += xdis;
		position->pos_y += ydis;
		position->rect.x = (int)position->pos_x;
		position->rect.y = (int)position->pos_y;

		if (is_collision()) {
			death();
		}
	}
}

bool SpellComponent::is_collision() {
	PositionComponent *position = GetPosition(entity);
	if (!position)
		return false;

	const auto entity_vec = Environment::get().get_resource_manager()->get_map()->get_entity_grid()->get_cells(position->rect);

	for (auto &vec : entity_vec) {
		for (auto &e : *vec) {
			const int &type = e->get_type();
			if (type == TYPE_OBJECT ||
				type == TYPE_ENEMY ||
				type == TYPE_PLAYER) {
				if (collision(position->rect, GetPosition(e)->rect) &&
					e != caster && e->get_type() != caster->get_type()) {

					if (CombatComponent *combat = GetCombat(e)) {
						combat->apply_damage(calc_damage(), color);
					}

					return true;
				}
			}
		}
	}

	return false;
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

int SpellComponent::calc_damage() {
	if (CombatComponent *combat = GetCombat(entity)) {
		return combat->damage + damage;
	}

	return damage;
}