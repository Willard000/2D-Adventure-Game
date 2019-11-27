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

// void Magic:cast(float x, float y)
static int magic_cast(lua_State *L) {
	MagicComponent *magic = luaW_check<MagicComponent>(L, -3);
	float x = (float)luaL_checknumber(L, -2);
	float y = (float)luaL_checknumber(L, -1);
	magic->cast_main(x, y);
	return 0;
}

// void Magic:reset_cast_time()
static int magic_reset_cast_time(lua_State *L) {
	MagicComponent *magic = luaW_check<MagicComponent>(L, -1);
	magic->can_cast = true;
	return 0;
}

// void Magic:stop_cast_time()
static int magic_stop_cast_time(lua_State *L) {
	MagicComponent *magic = luaW_check<MagicComponent>(L, -1);
	magic->can_cast = false;
	return 0;
}

static luaL_Reg magic_table[] = {
	{NULL, NULL}
};

static luaL_Reg magic_metatable[] = {
	// func
	{"cast", magic_cast},
	{"reset_cast_time", magic_reset_cast_time},
	{"stop_cast_time", magic_stop_cast_time},

	// set

	// get
	{"get_entity", magic_get_entity},

	{NULL, NULL}
};

void lua_init_magic(Lua *lua, lua_State *L) {
	luaW_register<MagicComponent>(L, "Magic", magic_table, magic_metatable, magic_new);
}