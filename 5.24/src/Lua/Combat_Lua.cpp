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

// void Combat:set_health(int val)
static int combat_set_health(lua_State *L) {
	CombatComponent *combat = luaW_check<CombatComponent>(L, -2);
	int val = luaL_checkinteger(L, -1);
	combat->health = val;
	if (combat->health > combat->max_health) {
		combat->health = combat->max_health;
	}
	return 0;
}

// int Combat:get_health()
static int combat_get_health(lua_State *L) {
	CombatComponent *combat = luaW_check<CombatComponent>(L, -1);
	lua_pushnumber(L, combat->health);
	return 1;
}

// int Combat:get_max_health()
static int combat_get_max_health(lua_State *L) {
	CombatComponent *combat = luaW_check<CombatComponent>(L, -1);
	lua_pushnumber(L, combat->max_health);
	return 1;
}

// void Combat:set_manaint val)
static int combat_set_mana(lua_State *L) {
	CombatComponent *combat = luaW_check<CombatComponent>(L, -2);
	int val = luaL_checkinteger(L, -1);
	combat->mana = val;
	if (combat->mana > combat->max_mana) {
		combat->mana = combat->max_mana;
	}
	return 0;
}

// int Combat:get_mana()
static int combat_get_mana(lua_State *L) {
	CombatComponent *combat = luaW_check<CombatComponent>(L, -1);
	lua_pushnumber(L, combat->mana);
	return 1;
}

// int Combat:get_max_mana()
static int combat_get_max_mana(lua_State *L) {
	CombatComponent *combat = luaW_check<CombatComponent>(L, -1);
	lua_pushnumber(L, combat->max_mana);
	return 1;
}

static luaL_Reg combat_table[] = {
	{NULL, NULL}
};

static luaL_Reg combat_metatable[] = {
	// func

	// set
	{"set_health", combat_set_health},
	{"set_mana", combat_set_mana},

	// get
	{"get_entity", combat_get_entity},
	{"get_health", combat_get_health},
	{"get_max_health", combat_get_max_health},
	{"get_mana", combat_get_mana},
	{"get_max_mana", combat_get_max_mana},
	{"in_combat", combat_in_combat},

	{NULL, NULL}
};

void lua_init_combat(Lua *lua, lua_State *L) {
	luaW_register<CombatComponent>(L, "Combat", combat_table, combat_metatable, combat_new);
}