#include "EnemyComponent.h"

#include "Environment.h"
#include "_Lua.h"
#include "ResourceManager.h"
#include "Window.h"

#include "Entity.h"
#include "CombatComponent.h"

EnemyComponent::EnemyComponent(Entity *entity_, std::string name_, std::string script_, Combat_Range combat_range_) :
	Component		( entity_ ),
	name			( name_ ),
	script			( script_ ),
	combat_range	( {combat_range_.w, combat_range_.h} ),
	in_combat		( false )
{
	Environment::get().get_lua()->load_script(script);
}

EnemyComponent::EnemyComponent(Entity *new_entity, const EnemyComponent &rhs) :
	Component	    ( new_entity ),
	name			( rhs.name ),
	script			( rhs.script ),
	combat_range	( {rhs.combat_range.w, rhs.combat_range.h} ),
	in_combat		( rhs.in_combat )
{}

EnemyComponent *EnemyComponent::copy(Entity *new_entity) const {
	return new EnemyComponent(new_entity, *this);
}

void EnemyComponent::update() {
	lua_State *L = Environment::get().get_lua()->get_state();
	lua_getglobal(L, name.c_str());
	lua_getfield(L, -1, "update");
	lua_remove(L, -2);
	luaW_push<EnemyComponent>(L, this);
	lua_pcall(L, 1, 0, 0);

	if (CombatComponent *combat = GetCombat(entity)) {
		if (!combat->in_combat) {
			combat->in_combat = check_combat_range();
		}
	}
}

bool EnemyComponent::is_collision() {
	PositionComponent *position = GetPosition(entity);
	if (!position)
		return false;

	const auto entity_vec = Environment::get().get_resource_manager()->get_map()->get_entity_grid()->get_cells(position->rect);

	for (auto &vec : entity_vec) {
		for (auto &e : *vec) {
			const int &type = e->get_type();
			if (type == TYPE_OBJECT ||
				type == TYPE_PLAYER ||
				type == TYPE_ENEMY) {
				if (collision(position->rect, GetPosition(e)->rect) &&
					e != entity)
					return true;
			}
		}
	}

	if (Environment::get().get_resource_manager()->get_map()->solid_collision(position->rect) ||
		Environment::get().get_resource_manager()->get_map()->bound_collision(position->rect))
		return true;

	return false;
}

bool EnemyComponent::check_combat_range() {
	PositionComponent *position = GetPosition(entity);
	if (!position) {
		return false;
	}

	PositionComponent *player_pos = GetPosition(Environment::get().get_resource_manager()->get_player());
	if (!player_pos) {
		return false;
	}

	SDL_Rect range = { position->rect.x - combat_range.w / 2, position->rect.y - combat_range.h / 2, combat_range.w, combat_range.h };

	if (collision(range, player_pos->rect)) {
		CombatComponent *combat = GetCombat(entity);

		if (!combat) {
			return false;
		}

		combat->start_combat();

		return true;
	}

	return false;
}