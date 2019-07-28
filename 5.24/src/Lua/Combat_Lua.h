#include "luawrapper.hpp"

#ifndef COMBAT_LUA_H
#define COMBAT_LUA_H

class Lua;

void lua_init_combat(Lua *lua, lua_State *L);

#endif