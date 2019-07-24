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

// void Sprite:set_ani(Uint8 ani)
static int sprite_set_ani(lua_State *L) {
	SpriteComponent *sprite = luaW_check<SpriteComponent>(L, -2);
	Uint8 ani = (Uint8)luaL_checknumber(L, -1);
	sprite->ani = ani;
	return 0;
}

static luaL_Reg sprite_table[] = {
	{NULL, NULL}
};

static luaL_Reg sprite_metatable[] = {
	// func

	// set
	{"set_ani", sprite_set_ani},

	// get
	{"get_entity", sprite_get_entity},

	{NULL, NULL}
};

void lua_init_sprite(Lua *lua, lua_State *L) {
	luaW_register<SpriteComponent>(L, "Sprite", sprite_table, sprite_metatable, sprite_new);
}