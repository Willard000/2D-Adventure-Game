#include "Player_Lua.h"

#include "Player.h"

#include "Environment.h"
#include "ResourceManager.h"
#include "Lua.h"

#include "PositionComponent.h"

#include "Entity_Lua.h"

// Member Functions
// Lua Constructor
// Player:new(int type_id)
static Player *player_new(lua_State *L) {
	int type_id = (int)luaL_checknumber(L, -1);
	return new Player(type_id);
}

// Player:get_type_id()
static int player_get_type_id(lua_State *L) {
	Player *player = luaW_check<Player>(L, -1);
	lua_pushnumber(L, player->get_type_id());
	return 1;
}

// Player:move(int direction)
// Player:move(int direction, float distance)
static int player_move(lua_State *L) {
	Player *player = luaW_check<Player>(L, 1);
	int direction = (int)lua_tonumber(L, 2);
	float distance = (float)lua_tonumber(L, 3);

	if (distance == 0) {
		GetPosition(player)->move(direction);
	}
	else {
		GetPosition(player)->move(direction, distance);
	}

	return 0;
}

// Player:scale(float factor)
static int player_scale(lua_State *L) {
	Player *player = luaW_check<Player>(L, 1);
	float factor = (float)lua_tonumber(L, 2);

	GetPosition(player)->scale(factor);
	return 0;
}

// Player:set_scale(float factor)
static int player_set_scale(lua_State *L) {
	Player *player = luaW_check<Player>(L, 1);
	float factor = (float)lua_tonumber(L, 2);

	GetPosition(player)->set_scale(factor);
	return 0;
}

// Player:rotate(double angle)
static int player_rotate(lua_State *L) {
	Player *player = luaW_check<Player>(L, 1);
	double angle = (double)lua_tonumber(L, 2);

	GetPosition(player)->rotate(angle);
	return 0;
}

// Player:set_rotation(double angle)
static int player_set_rotation(lua_State *L) {
	Player *player = luaW_check<Player>(L, 1);
	double angle = (double)lua_tonumber(L, 2);

	GetPosition(player)->set_rotation(angle);
	return 0;
}

static luaL_Reg player_table[] = {
	{NULL, NULL}
};

static luaL_Reg player_metatable[] = {
	{"get_type_id", player_get_type_id},
	{"move", player_move},
	{"scale", player_scale},
	{"set_scale", player_set_scale},
	{"rotate", player_rotate},
	{"set_rotation", player_set_rotation},
	{NULL, NULL}
};

// Global Functions
// GetPlayer()
static int player_get(lua_State *L) {
	luaW_push<Player>(L, Environment::get().get_resource_manager()->get_player());
	return 1;
}

void lua_init_player(Lua *lua, lua_State *L) {
	luaW_register<Player>(L, "Player", player_table, player_metatable, player_new);

	lua->registerGlobal("get_player", player_get);
}