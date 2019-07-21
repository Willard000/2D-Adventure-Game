#include "Player_Lua.h"

#include "PlayerComponent.h"

// not implemented
static PlayerComponent *player_new(lua_State *L) {
	return nullptr;
}

// Entity Player:get_entity()
static int player_get_entity(lua_State *L) {
	PlayerComponent *player = luaW_check<PlayerComponent>(L, -1);
	luaW_push<Entity>(L, player->entity);
	return 1;
}

static luaL_Reg player_table[] = {
	{NULL, NULL}
};

static luaL_Reg player_metatable[] = {
	// func

	// set

	// get
	{"get_entity", player_get_entity},

	{NULL, NULL}
};

void lua_init_player(Lua *lua, lua_State *L) {
	luaW_register<PlayerComponent>(L, "Player", player_table, player_metatable, player_new);
}