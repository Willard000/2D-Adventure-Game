#include "lua5.1/luawrapper.hpp"

#ifndef GLOBALS_LUA_H
#define GLOBALS_LUA_H

class ScriptManager;

void lua_init_globals(ScriptManager *scriptManager, lua_State *L);

#endif