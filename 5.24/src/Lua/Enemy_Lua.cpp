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

// int Enemy:get_move() 
static int enemy_get_move(lua_State *L) {
	EnemyComponent *enemy = luaW_check<EnemyComponent>(L, -1);
	lua_pushinteger(L, enemy->move);
	return 1;
}

// void Enemy:set_move(int num)
static int enemy_set_move(lua_State *L) {
	EnemyComponent *enemy = luaW_check<EnemyComponent>(L, -2);
	int move = luaL_checkinteger(L, -1);
	enemy->move = move;
	return 0;
}

static luaL_Reg enemy_table[] = {
	{NULL, NULL}
};

static luaL_Reg enemy_metatable[] = {
	// func

	// set
	{"set_move", enemy_set_move},

	// get
	{"get_move", enemy_get_move},
	{"get_entity", enemy_get_entity},

	{NULL, NULL}
};

void lua_init_enemy(Lua *lua, lua_State *L) {
	luaW_register<EnemyComponent>(L, "Enemy", enemy_table, enemy_metatable, enemy_new);
}