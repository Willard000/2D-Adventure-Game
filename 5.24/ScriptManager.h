#include <string>
#include <stdio.h>
#include <cassert>

#include "lua5.1/luawrapper.hpp"

#include "Player_Lua.h"

#ifndef SCRIPT_MANAGER_H
#define SCRIPT_MANAGER_H

class ScriptManager {
public:
	ScriptManager() {
		_L = lua_open();
		luaL_openlibs(_L);
		lua_openPlayer(_L);
	}

	~ScriptManager() {
		lua_close(_L);
	}

	void run(std::string filename) { 
		//printf("Running script: %s\n", filename.c_str());
		luaL_dofile(_L, filename.c_str());
	}

	template <class T>
	void registerGlobal(std::string name, T *obj) { 
		luaW_push<T>(_L, obj);
		lua_setglobal(_L, name.c_str());
	}

	void removeGlobal(std::string name) {
		lua_pushnil(_L);
		lua_setglobal(_L, name.c_str());
		lua_pop(_L, -1);
	}

private:
	lua_State *_L;
};

#endif