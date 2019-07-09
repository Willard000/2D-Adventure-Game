#include "lua5.1/luawrapper.hpp"

#include "Entity.h"
#include "PositionComponent.h"

#include "Environment.h"
#include "Log.h"

#ifndef ENTITY_LUA_H
#define ENTITY_LUA_H

class ScriptManager;

void lua_init_entity(ScriptManager *scriptManager, lua_State *L);

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

// Entity:set_x(double x)
static int entity_set_x(lua_State *L) {
	Entity *entity = luaW_check<Entity>(L, -2);
	double x = (double)luaL_checknumber(L, -1);
	if (PositionComponent *position = GetPosition(entity)) {
		position->pos_x = x;
		position->rect.x = (int)x;
		return 0;
	}
	Environment::get().get_log()->print("lua error - 'entity_set_x()' doesnt have position component");
	return 0;
}

// Entity:set_y(double x)
static int entity_set_y(lua_State *L) {
	Entity *entity = luaW_check<Entity>(L, -2);
	double y = (double)luaL_checknumber(L, -1);
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

#endif