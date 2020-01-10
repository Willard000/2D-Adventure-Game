#include "NPCComponent.h"

#include "Environment.h"
#include "ResourceManager.h"
#include "_Lua.h"

NPCComponent::NPCComponent(Entity *entity_, std::string script_name_, std::string script_, int quest_id_) :
	Component		( entity_ ),
	script_name		( script_name_ ),
	script			( script_ ),
	quest_id		( quest_id_ ),
	current_line	( 0 )
{
	Environment::get().get_lua()->load_script(script);
}

NPCComponent::NPCComponent(Entity *new_entity_, const NPCComponent &rhs) :
	Component		( new_entity_ ),
	script_name		( rhs.script_name ),
	script			( rhs.script ),
	quest_id		( rhs.quest_id ),
	current_line	( rhs.current_line )
{}

NPCComponent *NPCComponent::copy(Entity *new_entity) const {
	return new NPCComponent(new_entity, *this);
}

void NPCComponent::update() {
	update_path(pathing, GetPosition(entity));

	lua_State *L = Environment::get().get_lua()->get_state();
	lua_getglobal(L, script_name.c_str());
	lua_getfield(L, -1, "update");
	lua_remove(L, -2);
	luaW_push<NPCComponent>(L, this);
	lua_pcall(L, 1, 0, 0);
}


bool NPCComponent::is_collision() {
	PositionComponent *position = GetPosition(entity);
	if (!position)
		return false;

	const auto entity_vec = Environment::get().get_resource_manager()->get_map()->get_entity_grid()->get_cells(position->rect);

	for (auto &vec : entity_vec) {
		for (auto &e : *vec) {
			const int &type = e->get_type();
			if (type == TYPE_OBJECT ||
				type == TYPE_PLAYER ||
				type == TYPE_ENEMY  ||
				type == TYPE_NPC) {
				if (collision(position->rect, GetPosition(e)->rect, position->rotation, GetPosition(e)->rotation) &&
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