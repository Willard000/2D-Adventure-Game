#include "Sprite_Lua.h"

#include "MagicComponent.h"

// not implemented
static MagicComponent *magic_new(lua_State *L) {
	return nullptr;
}

// Entity Magic:get_entity()
static int magic_get_entity(lua_State *L) {
	MagicComponent *magic = luaW_check<MagicComponent>(L, -1);
	luaW_push<Entity>(L, magic->entity);
	return 1;
}

static luaL_Reg magic_table[] = {
	{NULL, NULL}
};

static luaL_Reg magic_metatable[] = {
	// func

	// set

	// get
	{"get_entity", magic_get_entity},

	{NULL, NULL}
};

void lua_init_magic(Lua *lua, lua_State *L) {
	luaW_register<MagicComponent>(L, "Magic", magic_table, magic_metatable, magic_new);
}