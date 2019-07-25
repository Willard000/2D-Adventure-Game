#include "EnemyComponent.h"

#include "Environment.h"
#include "_Lua.h"
#include "ResourceManager.h"

EnemyComponent::EnemyComponent(Entity *entity_, std::string name_, std::string script_) :
	Component		( entity_ ),
	name			( name_ ),
	script			( script_ )
{
	Environment::get().get_lua()->load_script(script);
}

void EnemyComponent::update() {
	lua_State *L = Environment::get().get_lua()->get_state();
	lua_getglobal(L, name.c_str());
	lua_getfield(L, -1, "update");
	lua_remove(L, -2);
	luaW_push<EnemyComponent>(L, this);
	lua_pcall(L, 1, 0, 0);
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