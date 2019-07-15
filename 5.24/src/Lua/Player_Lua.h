#include "luawrapper.hpp"

#ifndef PLAYER_LUA_H
#define PLAYER_LUA_H

class Lua;

void lua_init_player(Lua *lua, lua_State *L);

#endif