#include "lua5.1/luawrapper.hpp"

#include "Player.h"
#include "MoveableComponent.h"

#include "Environment.h"

#ifndef PLAYER_LUA_H
#define PLAYER_LUA_H

inline Player *Player_new(lua_State *L) {
	int type_id = (int)luaL_checknumber(L, -1);
	return new Player(type_id);
}

inline int Player_get_type_id(lua_State *L) {
	Player *player = luaW_check<Player>(L, -1);
	lua_pushnumber(L, player->get_type_id());
	return 1;
}

inline int Player_move(lua_State *L) {
	Player *player = luaW_check<Player>(L, 1);
	int direction = (int)lua_tonumber(L, 2);
	GetMoveable(player)->move(direction);
	return 0;
}

static luaL_Reg Player_table[] = {
	{NULL, NULL}
};

static luaL_Reg Player_metatable[] = {
	{"get_type_id", Player_get_type_id},
	{"move", Player_move},
	{NULL, NULL}
};

inline static int lua_openPlayer(lua_State *L) {
	luaW_register<Player>(L, "Player", Player_table, Player_metatable, Player_new);
	return 1;
}

#endif