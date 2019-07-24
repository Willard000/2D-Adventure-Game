#include "Effect_Lua.h"

#include "EffectComponent.h"

// not implemented
static EffectComponent *effect_new(lua_State *L) {
	return nullptr;
}

// Entity Effect:get_entity()
static int effect_get_entity(lua_State *L) {
	EffectComponent *effect = luaW_check<EffectComponent>(L, -1);
	luaW_push<Entity>(L, effect->entity);
	return 1;
}

static luaL_Reg effect_table[] = {
	{NULL, NULL}
};

static luaL_Reg effect_metatable[] = {
	// func

	// set

	// get
	{"get_entity", effect_get_entity},

	{NULL, NULL}
};

void lua_init_effect(Lua *lua, lua_State *L) {
	luaW_register<EffectComponent>(L, "Effect", effect_table, effect_metatable, effect_new);
}