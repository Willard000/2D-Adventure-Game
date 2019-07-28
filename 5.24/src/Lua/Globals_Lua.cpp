#include "Globals_Lua.h"

#include "Environment.h"
#include "_Lua.h"
#include "InputManager.h"
#include "Window.h"
#include "Clock.h"

#include "Entity.h"
#include "ResourceManager.h"

#include "Collision.h"


// int get_mouse_x()
static int mouse_get_x(lua_State *L) {
	int x = Environment::get().get_input_manager()->get_mouse_x();
	Camera *camera = Environment::get().get_window()->get_camera();
	x = int(x / float(camera->get_scale()) + camera->get_x());

	lua_pushnumber(L, x);
	return 1;
}

// int get_mouse_y()
static int mouse_get_y(lua_State *L) {
	int y = Environment::get().get_input_manager()->get_mouse_y();
	Camera *camera = Environment::get().get_window()->get_camera();
	y = int(y / float(camera->get_scale()) + camera->get_y());

	lua_pushnumber(L, y);
	return 1;
}

// double get_time()
static int get_time(lua_State *L) {
	lua_pushnumber(L, Environment::get().get_clock()->get_time());
	return 1;
}

// Entity get_player()
static int get_player(lua_State *L) {
	luaW_push<Entity>(L, Environment::get().get_resource_manager()->get_player());
	return 1;
}

// bool collision(PositionComponent *a, PositionComponent *b)
static int collision(lua_State *L) {
	PositionComponent *a = luaW_check<PositionComponent>(L, -2);
	PositionComponent *b = luaW_check<PositionComponent>(L, -1);
	bool is_collision = collision(a->rect, b->rect);
	lua_pushboolean(L, is_collision);
	return 1;
}

void lua_init_globals(Lua *lua, lua_State *L) {
	lua->register_global("get_mouse_x", mouse_get_x);
	lua->register_global("get_mouse_y", mouse_get_y);
	lua->register_global("get_time", get_time);
	lua->register_global("get_player", get_player);
	lua->register_global("collision", collision);
}