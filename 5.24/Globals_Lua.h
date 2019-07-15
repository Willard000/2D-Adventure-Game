#include "lua5.1/luawrapper.hpp"

#ifndef GLOBALS_LUA_H
#define GLOBALS_LUA_H

class Lua;

void lua_init_globals(Lua *lua, lua_State *L);

#endif