#include "luawrapper.hpp"

#include "Entity.h"
#include "PositionComponent.h"

#include "Environment.h"
#include "Log.h"

#ifndef ENTITY_LUA_H
#define ENTITY_LUA_H

class Lua;

void lua_init_entity(Lua *lua, lua_State *L);

// Entity:get_x()
static int entity_get_x(lua_State *L) {
	Entity *entity = luaW_check<Entity>(L, -1);
	if (PositionComponent *position = GetPosition(entity)) {
		lua_pushnumber(L, position->pos_x);
		return 1;
	}
	Environment::get().get_log()->print("lua error - 'entity_get_x()' doesnt have position component");
	return 0;
}

// Entity:get_y()
static int entity_get_y(lua_State *L) {
	Entity *entity = luaW_check<Entity>(L, -1);
	if (PositionComponent *position = GetPosition(entity)) {
		lua_pushnumber(L, position->pos_y);
		return 1;
	}
	Environment::get().get_log()->print("lua error - 'entity_get_y()' doesnt have position component");
	return 0;
}

// Entity:get_w()
static int entity_get_w(lua_State *L) {
	Entity *entity = luaW_check<Entity>(L, -1);
	if (PositionComponent *position = GetPosition(entity)) {
		lua_pushnumber(L, position->rect.w);
		return 1;
	}
	Environment::get().get_log()->print("lua error - 'entity_get_w()' doesnt have position component");
	return 0;
}

// Entity:get_h()
static int entity_get_h(lua_State *L) {
	Entity *entity = luaW_check<Entity>(L, -1);
	if (PositionComponent *position = GetPosition(entity)) {
		lua_pushnumber(L, position->rect.h);
		return 1;
	}
	Environment::get().get_log()->print("lua error - 'entity_get_h()' doesnt have position component");
	return 0;
}

// Entity:set_x(float x)
static int entity_set_x(lua_State *L) {
	Entity *entity = luaW_check<Entity>(L, -2);
	float x = (float)luaL_checknumber(L, -1);
	if (PositionComponent *position = GetPosition(entity)) {
		position->pos_x = x;
		position->rect.x = (int)x;
		return 0;
	}
	Environment::get().get_log()->print("lua error - 'entity_set_x()' doesnt have position component");
	return 0;
}

// Entity:set_y(float y)
static int entity_set_y(lua_State *L) {
	Entity *entity = luaW_check<Entity>(L, -2);
	float y = (float)luaL_checknumber(L, -1);
	if (PositionComponent *position = GetPosition(entity)) {
		position->pos_y = y;
		position->rect.y = (int)y;
		return 0;
	}
	Environment::get().get_log()->print("lua error - 'entity_set_y()' doesnt have position component");
	return 0;
}

// Entity:destroy()
static int entity_destroy(lua_State *L) {
	Entity *entity = luaW_check<Entity>(L, -1);
	entity->destroy();
	return 0;
}

//Entity:scale(float factor)
static int entity_scale(lua_State *L) {
	Entity *entity = luaW_check<Entity>(L, 1);
	float factor = (float)lua_tonumber(L, 2);

	GetPosition(entity)->scale(factor);
	return 0;
}

// Entity:set_scale(float factor)
static int entity_set_scale(lua_State *L) {
	Entity *entity = luaW_check<Entity>(L, 1);
	float factor = (float)lua_tonumber(L, 2);

	GetPosition(entity)->set_scale(factor);
	return 0;
}

// Entity:rotate(double angle)
static int entity_rotate(lua_State *L) {
	Entity *entity = luaW_check<Entity>(L, 1);
	double angle = (double)lua_tonumber(L, 2);

	GetPosition(entity)->rotate(angle);
	return 0;
}

// Entity:set_rotation(double angle)
static int entity_set_rotation(lua_State *L) {
	Entity *entity = luaW_check<Entity>(L, 1);
	double angle = (double)lua_tonumber(L, 2);

	GetPosition(entity)->set_rotation(angle);
	return 0;
}

#endif