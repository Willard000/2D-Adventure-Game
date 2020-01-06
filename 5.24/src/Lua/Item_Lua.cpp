#include "Item_Lua.h"

#include "ItemComponent.h"

#include "_Lua.h"


// not implemented
static ItemComponent *item_new(lua_State *L) {
	return nullptr;
}

// Entity Item:get_entity()
static int item_get_entity(lua_State *L) {
	ItemComponent *item = luaW_check<ItemComponent>(L, -1);
	luaW_push<Entity>(L, item->entity);
	return 1;
}

// void Item:delete()
static int item_delete(lua_State *L) {
	ItemComponent *item = luaW_check<ItemComponent>(L, -1);
	if (item->is_stackable && item->stack_size > 1) {
		--item->stack_size;
	}
	else {
		item->destroy = true;
	}
	return 0;
}

static luaL_Reg item_table[] = {
	{NULL, NULL}
};

static luaL_Reg item_metatable[] = {
	// func
	{"delete", item_delete},

	// set

	// get
	{"get_entity", item_get_entity},

	{NULL, NULL}
};

void lua_init_item(Lua *lua, lua_State *L) {
	luaW_register<ItemComponent>(L, "Item", item_table, item_metatable, item_new);
}