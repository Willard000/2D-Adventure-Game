#include <string>
#include <unordered_map>
#include <stdio.h>

#include "luawrapper.hpp"

#include "Globals_Lua.h"
#include "Entity_lua.h"
#include "Position_Lua.h"
#include "Sprite_Lua.h"
#include "Spell_Lua.h"
#include "Magic_Lua.h"
#include "Player_Lua.h"
#include "Enemy_Lua.h"

#include "Environment.h"
#include "Log.h"

#ifndef _LUA_H
#define _LUA_H

class Lua {
public:
	Lua() :
		_L		( lua_open() )
	{
		luaL_openlibs(_L);

		lua_init_position(this, _L);
		lua_init_spell(this, _L);
		lua_init_sprite(this, _L);
		lua_init_magic(this, _L);
		lua_init_player(this, _L);
		lua_init_enemy(this, _L);

		lua_init_entity(this, _L);

		lua_init_globals(this, _L);  // do this after lua tables are made
	}

	~Lua() {
		lua_close(_L);
	}

	void run(std::string filename) { 
		Environment::get().get_log()->print("Running script: " + filename);
		luaL_dofile(_L, filename.c_str());
	}

	template <class T>
	void register_global(const char *name, T *obj) { 
		luaW_push<T>(_L, obj);
		lua_setglobal(_L, name);
		lua_pop(_L, -1);
	}

	void register_global(const char *name, lua_CFunction func) {
		lua_pushcfunction(_L, func);
		lua_setglobal(_L, name);
		lua_pop(_L, -1);
	}

	void remove_global(std::string name) {
		lua_pushnil(_L);
		lua_setglobal(_L, name.c_str());
		lua_pop(_L, -1);
	}

	void load_script(std::string &script) {
		if (!_loaded_scripts[script]) {
			luaL_dofile(_L, script.c_str());
			_loaded_scripts[script] = true;
		}
	}

	lua_State *get_state() { return _L; }

private:
	lua_State *_L;

	// Key - script_path, Val - is_loaded
	std::unordered_map<std::string, bool> _loaded_scripts;
};

#endif