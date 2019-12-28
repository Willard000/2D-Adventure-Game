#include "NPC_Lua.h"

#include "NPCComponent.h"

// not implemented
static NPCComponent *npc_new(lua_State *L) {
	return nullptr;
}

// Entity NPC:get_entity()
static int npc_get_entity(lua_State *L) {
	NPCComponent *npc = luaW_check<NPCComponent>(L, -1);
	luaW_push<Entity>(L, npc->entity);
	return 1;
}

// int NPC:get_current_line()
static int npc_get_current_line(lua_State *L) {
	NPCComponent *npc = luaW_check<NPCComponent>(L, -1);
	lua_pushnumber(L, npc->current_line);
	return 1;
}

// void NPC:set_current_line()
static int npc_set_current_line(lua_State *L) {
	NPCComponent *npc = luaW_check<NPCComponent>(L, -2);
	int current_line = luaL_checkinteger(L, -1);
	npc->current_line = current_line;
	return 0;
}

// int NPC:get_quest_id()
static int npc_get_quest_id(lua_State *L) {
	NPCComponent *npc = luaW_check<NPCComponent>(L, -1);
	lua_pushnumber(L, npc->quest_id);
	return 1;
}

static luaL_Reg npc_table[] = {
	{NULL, NULL}
};

static luaL_Reg npc_metatable[] = {
	// func

	// set
	{"set_current_line", npc_set_current_line},

	// get
	{"get_npc", npc_get_entity},
	{"get_current_line", npc_get_current_line},
	{"get_quest_id", npc_get_quest_id},

	{NULL, NULL}
};

void lua_init_npc(Lua *lua, lua_State *L) {
	luaW_register<NPCComponent>(L, "NPC", npc_table, npc_metatable, npc_new);
}