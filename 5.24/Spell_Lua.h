#include "lua5.1/luawrapper.hpp"

#include "Entity.h"

#ifndef SPELL_LUA_H
#define SPELL_LUA_H

class Lua;

void lua_init_spell(Lua *lua, lua_State *L);

#endif