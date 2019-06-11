#include <string>

#include "lua5.1/luawrapper.hpp"

extern "C" {

#include "lua5.1/include/lua.h"
#include "lua5.1/include/lualib.h"
#include "lua5.1/include/lauxlib.h"

}

#pragma comment(lib, "lua5.1/lib/dll/lua5.1.lib")

#ifndef LUA_H
#define LUA_H

class Lua {
public:
	Lua() {
		_L = lua_open();
		luaL_openlibs(_L);
		lua_register(_L, "test", Lua::test);
		execute("test", "Data/Lua/test.lua");
	}

	~Lua() {
		lua_close(_L);
	}

	void execute(std::string name, std::string filename) {
		luaL_dofile(_L, filename.c_str());
	}

	void subscribe(std::string name, lua_CFunction &function) {
		lua_register(_L, name.c_str(), function);
	}
private:
	lua_State *_L;

public:

	static int test(lua_State *L) {
		int n = lua_gettop(L);

		n += 20;

		lua_pushnumber(L, n);

		return 1;
	}
};

#endif