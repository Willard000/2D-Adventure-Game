#include "Globals_Lua.h"

#include "Environment.h"
#include "ScriptManager.h"
#include "InputManager.h"
#include "Window.h"
#include "Clock.h"


// get_mouse_x()
static int mouse_get_x(lua_State *L) {
	int x = Environment::get().get_input_manager()->get_mouse_x();
	Camera *camera = Environment::get().get_window()->get_camera();
	x = int(double((x / camera->get_scale()) + camera->get_x()));

	lua_pushnumber(L, x);
	return 1;
}

// get_mouse_y()
static int mouse_get_y(lua_State *L) {
	int y = Environment::get().get_input_manager()->get_mouse_y();
	Camera *camera = Environment::get().get_window()->get_camera();
	y = int(double((y / camera->get_scale()) + camera->get_y()));

	lua_pushnumber(L, y);
	return 1;
}

static int get_time(lua_State *L) {
	lua_pushnumber(L, Environment::get().get_clock()->get_time());
	return 1;
}

void lua_init_globals(ScriptManager *scriptManager, lua_State *L) {
	scriptManager->registerGlobal("get_mouse_x", mouse_get_x);
	scriptManager->registerGlobal("get_mouse_y", mouse_get_y);
	scriptManager->registerGlobal("get_time", get_time);
}