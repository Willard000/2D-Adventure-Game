#include "luawrapper.hpp"

#ifndef EFFECT_LUA_H
#define EFFECT_LUA_H

class Lua;

void lua_init_effect(Lua *lua, lua_State *L);

#endif