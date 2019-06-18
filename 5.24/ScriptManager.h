#include <string>
#include <stdio.h>

#include "lua5.1/luawrapper.hpp"

#include "Player_Lua.h"

#ifndef SCRIPT_MANAGER_H
#define SCRIPT_MANAGER_H

class ScriptManager {
public:
	ScriptManager() :
		_L ( lua_open() )
	{
		luaL_openlibs(_L);
		lua_init_player(this, _L);
	}

	~ScriptManager() {
		lua_close(_L);
	}

	void run(std::string filename) { 
		//printf("Running script: %s\n", filename.c_str());
		luaL_dofile(_L, filename.c_str());
	}

	template <class T>
	void registerGlobal(const char *name, T *obj) { 
		luaW_push<T>(_L, obj);
		lua_setglobal(_L, name);
	}

	void registerGlobal(const char *name, lua_CFunction func) {
		lua_pushcfunction(_L, func);
		lua_setglobal(_L, name);
	}

	void removeGlobal(std::string name) {
		lua_pushnil(_L);
		lua_setglobal(_L, name.c_str());
	}

private:
	lua_State *_L;
};

#endif