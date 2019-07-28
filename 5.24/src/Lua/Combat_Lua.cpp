#include "Combat_Lua.h"

#include "CombatComponent.h"

// not implemented
static CombatComponent *combat_new(lua_State *L) {
	return nullptr;
}

// Entity Combat:get_entity()
static int combat_get_entity(lua_State *L) {
	CombatComponent *combat = luaW_check<CombatComponent>(L, -1);
	luaW_push<Entity>(L, combat->entity);
	return 1;
}

// bool Combat:is_in_combat()
static int combat_in_combat(lua_State *L) {
	CombatComponent *combat = luaW_check<CombatComponent>(L, -1);
	lua_pushboolean(L, combat->in_combat);
	return 1;
}

static luaL_Reg combat_table[] = {
	{NULL, NULL}
};

static luaL_Reg combat_metatable[] = {
	// func

	// set

	// get
	{"get_entity", combat_get_entity},
	{"in_combat", combat_in_combat},

	{NULL, NULL}
};

void lua_init_combat(Lua *lua, lua_State *L) {
	luaW_register<CombatComponent>(L, "Combat", combat_table, combat_metatable, combat_new);
}