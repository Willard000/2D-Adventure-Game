#include "luawrapper.hpp"

#ifndef POSITION_LUA_H
#define POSITION_LUA_H

class Lua;

void lua_init_position(Lua *lua, lua_State *L);

#endif