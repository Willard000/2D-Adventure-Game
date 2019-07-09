#include "lua5.1/luawrapper.hpp"

#include "Entity.h"

#ifndef SPELL_LUA_H
#define SPELL_LUA_H

class ScriptManager;

void lua_init_spell(ScriptManager *scriptManager, lua_State *L);

#endif