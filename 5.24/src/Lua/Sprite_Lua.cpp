#include "Sprite_Lua.h"

#include "SpriteComponent.h"

// not implemented
static SpriteComponent *sprite_new(lua_State *L) {
	return nullptr;
}

// Entity Sprite:get_entity()
static int sprite_get_entity(lua_State *L) {
	SpriteComponent *sprite = luaW_check<SpriteComponent>(L, -1);
	luaW_push<Entity>(L, sprite->entity);
	return 1;
}

static luaL_Reg sprite_table[] = {
	{NULL, NULL}
};

static luaL_Reg sprite_metatable[] = {
	// func

	// set

	// get
	{"get_entity", sprite_get_entity},

	{NULL, NULL}
};

void lua_init_sprite(Lua *lua, lua_State *L) {
	luaW_register<SpriteComponent>(L, "Sprite", sprite_table, sprite_metatable, sprite_new);
}