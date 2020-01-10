#include "Commands_Lua.h"

#include "Environment.h"
#include "_Lua.h"
#include "ResourceManager.h"

#include "Component.h"
#include "PlayerComponent.h"

// void teleport(int map_id)
static int teleport(lua_State *L) {
	int map_id = (int)luaL_checknumber(L, -1);

	Environment::get().get_resource_manager()->get_map()->save(false);
	Environment::get().get_resource_manager()->load_map(map_id, false);

	return 0;
}

// void teleport_to(int map_id, int x, int y, int w, int h)
static int teleport_to(lua_State *L) {
	int map_id = (int)luaL_checknumber(L, -5);
	int x = (int)luaL_checknumber(L, -4);
	int y = (int)luaL_checknumber(L, -3);
	int w = (int)luaL_checknumber(L, -2);
	int h = (int)luaL_checknumber(L, -1);

	GetPlayer(Environment::get().get_resource_manager()->get_player())->warp(map_id, { x, y, w, h });

	return 0;
}

// void player_collision()
static int player_collision(lua_State *L) {
	PlayerComponent *player = GetPlayer(Environment::get().get_resource_manager()->get_player());
	player->debug_collision = !player->debug_collision;

	if (player->debug_collision) {
		Environment::get().get_resource_manager()->get_textbox().print("Player Collision Off");
		printf("Player Collision Off");
	}
	else {
		Environment::get().get_resource_manager()->get_textbox().print("Player Collision On");
		printf("Player Collision On");
	}

	return 0;
}

// void reload_all()
// reloads all maps to their base
static int reload_all(lua_State *L) {
	Environment::get().get_resource_manager()->get_map()->reload_all();
	return 0;
}

void lua_init_commands(Lua *lua, lua_State *L) {
	lua->register_global("teleport", teleport);
	lua->register_global("teleport_to", teleport_to);
	lua->register_global("tcl", player_collision);
	lua->register_global("reload_all", reload_all);
}