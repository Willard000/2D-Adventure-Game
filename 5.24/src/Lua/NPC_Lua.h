#include "luawrapper.hpp"

#ifndef NPC_LUA_H
#define NPC_LUA_H

class Lua;

void lua_init_npc(Lua *lua, lua_State *L);

#endif