#include "lua5.1/luawrapper.hpp"

#ifndef PLAYER_LUA_H
#define PLAYER_LUA_H

class ScriptManager;

void lua_init_player(ScriptManager *scriptManager, lua_State *L);

#endif