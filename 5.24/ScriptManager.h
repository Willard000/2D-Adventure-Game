#include <string>
#include <stdio.h>

#include "lua5.1/luawrapper.hpp"

#include "Globals_Lua.h"
#include "Player_Lua.h"
#include "Spell_Lua.h"

#include "Environment.h"
#include "Log.h"

#ifndef SCRIPT_MANAGER_H
#define SCRIPT_MANAGER_H

class ScriptManager {
public:
	ScriptManager() :
		_L ( lua_open() )
	{
		luaL_openlibs(_L);

		lua_init_globals(this, _L);
		lua_init_player(this, _L);
		lua_init_spell(this, _L);
	}

	~ScriptManager() {
		lua_close(_L);
	}

	void run(std::string filename) { 
		Environment::get().get_log()->print("Running script: " + filename);
		luaL_dofile(_L, filename.c_str());
	}

	template <class T>
	void registerGlobal(const char *name, T *obj) { 
		luaW_push<T>(_L, obj);
		lua_setglobal(_L, name);
		lua_pop(_L, -1);
	}

	void registerGlobal(const char *name, lua_CFunction func) {
		lua_pushcfunction(_L, func);
		lua_setglobal(_L, name);
		lua_pop(_L, -1);
	}

	void removeGlobal(std::string name) {
		lua_pushnil(_L);
		lua_setglobal(_L, name.c_str());
	}

	lua_State *getL() { return _L; }

private:
	lua_State *_L;
};

#endif