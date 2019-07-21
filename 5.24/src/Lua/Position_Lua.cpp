#include "Position_Lua.h"

#include "PositionComponent.h"


// not implemented
static PositionComponent *position_new(lua_State *L) {
	int type_id = (int)luaL_checknumber(L, -1);
	return nullptr;
}

// Entity Position:get_entity()
static int position_get_entity(lua_State *L) {
	PositionComponent *position = luaW_check<PositionComponent>(L, -1);
	luaW_push<Entity>(L, position->entity);
	return 1;
}

// float Position:get_x()
static int position_get_x(lua_State *L) {
	PositionComponent *position = luaW_check<PositionComponent>(L, -1);
	lua_pushnumber(L, position->pos_x);
	return 1;
}

// float Position:get_y()
static int position_get_y(lua_State *L) {
	PositionComponent *position = luaW_check<PositionComponent>(L, -1);
	lua_pushnumber(L, position->pos_y);
	return 1;
}

// int Position:get_w()
static int position_get_w(lua_State *L) {
	PositionComponent *position = luaW_check<PositionComponent>(L, -1);
	lua_pushnumber(L, position->rect.w);
	return 1;
}

// int Position:get_h()
static int position_get_h(lua_State *L) {
	PositionComponent *position = luaW_check<PositionComponent>(L, -1);
	lua_pushnumber(L, position->rect.h);
	return 1;
}

// void Position:set(float x, float y)
static int position_set(lua_State *L) {
	PositionComponent *position = luaW_check<PositionComponent>(L, -3);
	float x = (float)luaL_checknumber(L, -2);
	float y = (float)luaL_checknumber(L, -1);
	position->set(x, y);
	return 0;
}

// void Position:scale(float factor)
static int position_scale(lua_State *L) {
	PositionComponent *position = luaW_check<PositionComponent>(L, -2);
	float factor = (float)lua_tonumber(L, -1);
	position->scale(factor);
	return 0;
}

// void Position:set_scale(float factor)
static int position_set_scale(lua_State *L) {
	PositionComponent *position = luaW_check<PositionComponent>(L, -2);
	float factor = (float)lua_tonumber(L, -1);
	position->set_scale(factor);
	return 0;
}

// void Position:rotate(double angle)
static int position_rotate(lua_State *L) {
	PositionComponent *position = luaW_check<PositionComponent>(L, -2);
	double angle = (double)lua_tonumber(L, -1);
	position->rotate(angle);
	return 0;
}

// void Position:set_rotate(double angle)
static int position_set_rotation(lua_State *L) {
	PositionComponent *position = luaW_check<PositionComponent>(L, -2);
	double angle = (double)lua_tonumber(L, -1);
	position->set_rotation(angle);
	return 0;
}

// void Position:move(int dir)
static int position_move(lua_State *L) {
	PositionComponent *position = luaW_check<PositionComponent>(L, -2);
	int dir = (int)luaL_checknumber(L, -1);
	position->move(dir);
	return 0;
}

// void Position:move_dis(int dir, float dis)
static int position_move_dis(lua_State *L) {
	PositionComponent *position = luaW_check<PositionComponent>(L, -3);
	int dir = (int)luaL_checknumber(L, -2);
	float dis = (float)luaL_checknumber(L, -1);
	position->move(dir, dis);
	return 0;
}

static luaL_Reg position_table[] = {
	{NULL, NULL}
};

static luaL_Reg position_metatable[] = {
	// func
	{"move", position_move},
	{"move_dis", position_move_dis},
	{"scale", position_scale},
	{"set_scale", position_set_scale},
	{"rotate", position_rotate},
	{"set_rotation", position_set_rotation},

	// set
	{"set", position_set},

	// get
	{"get_entity", position_get_entity},
	{"get_x", position_get_x},
	{"get_y", position_get_y},
	{"get_w", position_get_w},
	{"get_h", position_get_h},

	{NULL, NULL}
};

void lua_init_position(Lua *lua, lua_State *L) {
	luaW_register<PositionComponent>(L, "Position", position_table, position_metatable, position_new);
}