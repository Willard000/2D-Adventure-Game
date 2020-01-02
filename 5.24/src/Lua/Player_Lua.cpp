#include "Player_Lua.h"

#include "PlayerComponent.h"

// not implemented
static PlayerComponent *player_new(lua_State *L) {
	return nullptr;
}

// Entity Player:get_entity()
static int player_get_entity(lua_State *L) {
	PlayerComponent *player = luaW_check<PlayerComponent>(L, -1);
	luaW_push<Entity>(L, player->entity);
	return 1;
}

// void Player:activate_quest(int quest_id) 
static int player_activate_quest(lua_State *L) {
	PlayerComponent *player = luaW_check<PlayerComponent>(L, -2);
	int quest_id = luaL_checkinteger(L, -1);
	if (player->quest_log[quest_id] != nullptr) {
		player->quest_log[quest_id]->activate();
	}
	return 0;
}

// void player:complete_quest(int quest_id) 
static int player_complete_quest(lua_State *L) {
	PlayerComponent *player = luaW_check<PlayerComponent>(L, -2);
	int quest_id = luaL_checkinteger(L, -1);
	if (player->quest_log[quest_id] != nullptr) {
		player->quest_log[quest_id]->complete();
	}
	return 0;
}

// bool player:quest_should_complete(int quest_id) 
static int player_quest_should_complete(lua_State *L) {
	PlayerComponent *player = luaW_check<PlayerComponent>(L, -2);
	int quest_id = luaL_checkinteger(L, -1);
	if (player->quest_log[quest_id] != nullptr) {
		lua_pushboolean(L, player->quest_log[quest_id]->should_complete());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}

// bool player:quest_is_complete(int quest_id) 
static int player_quest_is_complete(lua_State *L) {
	PlayerComponent *player = luaW_check<PlayerComponent>(L, -2);
	int quest_id = luaL_checkinteger(L, -1);
	if (player->quest_log[quest_id] != nullptr) {
		lua_pushboolean(L, player->quest_log[quest_id]->is_complete());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}

// bool Player:has_quest(int quest_id)
static int player_has_quest(lua_State *L) {
	PlayerComponent *player = luaW_check<PlayerComponent>(L, -2);
	int quest_id = luaL_checkinteger(L, -1);
	if (player->quest_log[quest_id] != nullptr && player->quest_log[quest_id]->is_active()) {
		lua_pushboolean(L, 1);
		return 1;
	}
	lua_pushboolean(L, 0);
	return 1;
}

// bool Player:has_complete_quest(int quest_id)
static int player_has_complete_quest(lua_State *L) {
	PlayerComponent *player = luaW_check<PlayerComponent>(L, -2);
	int quest_id = luaL_checkinteger(L, -1);
	if (player->quest_log[quest_id] != nullptr && player->quest_log[quest_id]->is_complete()) {
		lua_pushboolean(L, 1);
		return 1;
	}
	lua_pushboolean(L, 0);
	return 1;
}

// bool Player:get_kill_count(int quest_id)
static int player_get_kill_count(lua_State *L) {
	PlayerComponent *player = luaW_check<PlayerComponent>(L, -2);
	int quest_id = luaL_checkinteger(L, -1);
	if (player->quest_log[quest_id] != nullptr && player->quest_log[quest_id]->get_type() == QUEST_TYPE_KILL) {
		lua_pushinteger(L, player->quest_log[quest_id]->get_kill_count());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}

// string Player::get_quest_info(quest_id)
static int player_get_quest_info(lua_State *L) {
	PlayerComponent *player = luaW_check<PlayerComponent>(L, -2);
	int quest_id = luaL_checkinteger(L, -1);
	if (player->quest_log[quest_id] != nullptr && player->quest_log[quest_id]->is_active()) {
		lua_pushstring(L, player->quest_log[quest_id]->get_info_string().c_str());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}

// void player:add_kill_count(int quest_id) 
static int player_add_kill_count(lua_State *L) {
	PlayerComponent *player = luaW_check<PlayerComponent>(L, -2);
	int quest_id = luaL_checkinteger(L, -1);
	if (player->quest_log[quest_id] != nullptr && player->quest_log[quest_id]->get_type() == QUEST_TYPE_KILL) {
		player->quest_log[quest_id]->add_kill();
	}
	return 0;
}

// void player:add_item(int item_id)
static int player_add_item(lua_State *L) {
	PlayerComponent *player = luaW_check<PlayerComponent>(L, -2);
	int item_id = luaL_checkinteger(L, -1);
	Entity *item = new Entity(TYPE_ITEM, item_id);
	player->items.push_back(item);
	return 0;
}

static luaL_Reg player_table[] = {
	{NULL, NULL}
};

static luaL_Reg player_metatable[] = {
	// func
	{"activate_quest", player_activate_quest},
	{"complete_quest", player_complete_quest},
	{"quest_should_complete", player_quest_should_complete},
	{"quest_is_complete", player_quest_is_complete},
	{"add_kill_count", player_add_kill_count},
	{"add_item", player_add_item},

	// set

	// get
	{"get_entity", player_get_entity},
	{"has_quest", player_has_quest},
	{"has_complete_quest", player_has_complete_quest},
	{"get_quest_info", player_get_quest_info},

	{NULL, NULL}
};

void lua_init_player(Lua *lua, lua_State *L) {
	luaW_register<PlayerComponent>(L, "Player", player_table, player_metatable, player_new);
}