#include "luawrapper.hpp"

#ifndef COMMANDS_LUA_H
#define COMMANDS_LUA_H

class Lua;

void lua_init_commands(Lua *lua, lua_State *L);

#endif