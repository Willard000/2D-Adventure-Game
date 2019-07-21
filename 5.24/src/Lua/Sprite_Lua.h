#include "luawrapper.hpp"

#ifndef SPRITE_LUA_H
#define SPRITE_LUA_H

class Lua;

void lua_init_sprite(Lua *lua, lua_State *L);

#endif