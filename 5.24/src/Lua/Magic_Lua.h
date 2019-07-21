#include "luawrapper.hpp"

#ifndef MAGIC_LUA_H
#define MAGIC_LUA_H

class Lua;

void lua_init_magic(Lua *lua, lua_State *L);

#endif