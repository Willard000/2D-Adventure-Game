#include "luawrapper.hpp"

#ifndef ITEM_LUA_H
#define ITEM_LUA_H

class Lua;

void lua_init_item(Lua *lua, lua_State *L);

#endif