#include "luawrapper.hpp"

#ifndef INTERACT_LUA_H
#define INTERACT_LUA_H

class Lua;

void lua_init_interact(Lua *lua, lua_State *L);

#endif