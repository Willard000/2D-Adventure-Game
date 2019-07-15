#include "Entity_Lua.h"

#include "Entity.h"
#include "PositionComponent.h"

#include "Environment.h"
#include "Log.h"

static Entity *entity_new(lua_State *L) {
	const char *type = luaL_checkstring(L, -2);
	int id = (int)luaL_checknumber(L, -1);
	return new Entity(type, id);
}

static luaL_Reg entity_table[] = {
	{NULL, NULL}
};

static luaL_Reg entity_metatable[] = {
	{"get_x", entity_get_x},
	{"get_y", entity_get_y},
	{"get_w", entity_get_w},
	{"get_h", entity_get_h},
	{"set_x", entity_set_x},
	{"set_y", entity_set_y},
	{NULL, NULL}
};

void lua_init_entity(Lua *lua, lua_State *L) {
	luaW_register<Entity>(L, "Entity", entity_table, entity_metatable, entity_new);
}