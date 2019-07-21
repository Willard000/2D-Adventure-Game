#include "Enemy_Lua.h"

#include "EnemyComponent.h"

// not implemented
static EnemyComponent *enemy_new(lua_State *L) {
	return nullptr;
}

// Entity Enemy:get_entity()
static int enemy_get_entity(lua_State *L) {
	EnemyComponent *enemy = luaW_check<EnemyComponent>(L, -1);
	luaW_push<Entity>(L, enemy->entity);
	return 1;
}

static luaL_Reg enemy_table[] = {
	{NULL, NULL}
};

static luaL_Reg enemy_metatable[] = {
	// func

	// set

	// get
	{"get_entity", enemy_get_entity},

	{NULL, NULL}
};

void lua_init_enemy(Lua *lua, lua_State *L) {
	luaW_register<EnemyComponent>(L, "Enemy", enemy_table, enemy_metatable, enemy_new);
}