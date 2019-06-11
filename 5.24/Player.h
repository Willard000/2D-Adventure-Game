#include <string>

#include "Entity.h"

#include "lua5.1/luawrapper.hpp"

#ifndef PLAYER_H
#define PLAYER_H

class Player : public Entity {
public:
	Player(int type_id) : Entity(TYPE_PLAYER, type_id) {};
private:
};

inline Player *Player_new(lua_State *L) {
	int type_id = (int)luaL_checknumber(L, -1);
	return new Player(type_id);
}

inline int Player_get_type_id(lua_State *L) {
	Player *player = luaW_check<Player>(L, -1);
	lua_pushnumber(L, player->get_type_id());
	return 1;
}

// Static Functions
// Name & Function Pointer
// Ends in NULL NULL
static luaL_Reg Player_table[] = {
	{NULL, NULL}
};

static luaL_Reg Player_metatable[] = {
	{"get_type_id", Player_get_type_id},
	{NULL, NULL}
};

inline static int lua_openPlayer(lua_State *L) {
	luaW_register<Player>(L, "Player", Player_table, Player_metatable, Player_new);
	return 1;
}

#endif