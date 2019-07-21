#include "luawrapper.hpp"

#ifndef ENEMY_LUA_H
#define ENEMY_LUA_H

class Lua;

void lua_init_enemy(Lua *lua, lua_State *L);

#endif