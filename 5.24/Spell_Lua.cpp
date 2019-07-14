#include "Spell_Lua.h"

#include "SpellComponent.h"
#include "PositionComponent.h"
#include "SpriteComponent.h"

#include "Environment.h"
#include "Log.h"
#include "ResourceManager.h"
#include "Clock.h"

#include "Player.h"

#include "Entity_Lua.h"

#include "Collision.h"

// spell_new
static Entity *spell_new(lua_State *L) {
	int id = (int)luaL_checknumber(L, -1);
	return new Entity(TYPE_SPELL, id);
}

// Spell:get_speed()
static int spell_get_speed(lua_State *L) {
	Entity *spell = luaW_check<Entity>(L, -1);
	if (SpellComponent *spell_comp = GetSpell(spell)) {
		lua_pushnumber(L, spell_comp->speed);
		return 1;
	}
	Environment::get().get_log()->print("lua error - 'spell_get_speed()' doesnt have spell component");
	return 0;
}

// Spell:set_dx(double dx)
static int spell_set_dx(lua_State *L) {
	Entity *spell = luaW_check<Entity>(L, -2);
	double dx = (double)luaL_checknumber(L, -1);
	if (SpellComponent *spell_comp = GetSpell(spell)) {
		spell_comp->dx = dx;
		return 0;
	}
	Environment::get().get_log()->print("lua error - 'spell_set_dx()' doesnt have spell component");
	return 0;
}

// Spell:set_dy(double dy)
static int spell_set_dy(lua_State *L) {
	Entity *spell = luaW_check<Entity>(L, -2);
	double dy = (double)luaL_checknumber(L, -1);
	if (SpellComponent *spell_comp = GetSpell(spell)) {
		spell_comp->dy = dy;
		return 0;
	}
	Environment::get().get_log()->print("lua error - 'spell_set_dy()' doesnt have spell component");
	return 0;
}

// Spell:get_dx()
static int spell_get_dx(lua_State *L) {
	Entity *spell = luaW_check<Entity>(L, -1);
	if (SpellComponent *spell_comp = GetSpell(spell)) {
		lua_pushnumber(L, spell_comp->dx);
		return 1;
	}

	Environment::get().get_log()->print("lua error - 'spell_get_dx()' doesnt have spell component");
	return 0;
}

// Spell:get_dy()
static int spell_get_dy(lua_State *L) {
	Entity *spell = luaW_check<Entity>(L, -1);
	if (SpellComponent *spell_comp = GetSpell(spell)) {
		lua_pushnumber(L, spell_comp->dy);
		return 1;
	}

	Environment::get().get_log()->print("lua error - 'spell_get_dy()' doesnt have spell component");
	return 0;
}

// Spell:set_dis(double dis)
static int spell_set_dis(lua_State *L) {
	Entity *spell = luaW_check<Entity>(L, -2);
	double dis = (double)luaL_checknumber(L, -1);
	if (SpellComponent *spell_comp = GetSpell(spell)) {
		spell_comp->dis = dis;
		return 0;
	}
	Environment::get().get_log()->print("lua error - 'spell_set_dis()' doesnt have spell component");
	return 0;
}

// Spell:get_dis()
static int spell_get_dis(lua_State *L) {
	Entity *spell = luaW_check<Entity>(L, -1);
	if (SpellComponent *spell_comp = GetSpell(spell)) {
		lua_pushnumber(L, spell_comp->dis);
		return 1;
	}

	Environment::get().get_log()->print("lua error - 'spell_get_dis()' doesnt have spell component");
	return 0;
}

// Spell:get_max_dis()
static int spell_get_max_dis(lua_State *L) {
	Entity *spell = luaW_check<Entity>(L, -1);
	if (SpellComponent *spell_comp = GetSpell(spell)) {
		lua_pushnumber(L, spell_comp->max_dis);
		return 1;
	}

	Environment::get().get_log()->print("lua error - 'spell_get_max_dis()' doesnt have spell component");
	return 0;
}

// Spell:get_owner()
static int spell_get_owner(lua_State *L) {
	Entity *spell = luaW_check<Entity>(L, -1);
	if (SpellComponent *spell_comp = GetSpell(spell)) {
		luaW_push<Entity>(L, spell_comp->owner);
		return 1;
	}
	Environment::get().get_log()->print("lua error - 'spell_get_owner()' doesnt have spell component");
	return 0;
}

// Spell:move(double x, double y)
static int spell_move(lua_State *L) {

	Entity *spell = luaW_check<Entity>(L, -3);
	double x = (double)luaL_checknumber(L, -2);
	double y = (double)luaL_checknumber(L, -1);
	double xdis = x * Environment::get().get_clock()->get_time();
	double ydis = y * Environment::get().get_clock()->get_time();

	SpellComponent *spell_comp = GetSpell(spell);
	spell_comp->dis += abs(xdis) + abs(ydis);

	if (PositionComponent *position = GetPosition(spell)) {
		position->pos_x += xdis;
		position->pos_y += ydis;
		position->rect.x = (int)position->pos_x;
		position->rect.y = (int)position->pos_y;
		if (Environment::get().get_resource_manager()->get_map()->entity_collision(position->rect)) {
			spell_comp->death();
		}
	}

	return 0;
}

// Spell:death()
static int spell_death(lua_State *L) {
	Entity *spell = luaW_check<Entity>(L, -1);
	if (SpellComponent *spell_comp = GetSpell(spell)) {
		spell_comp->death();
	}
	return 0;
}

static luaL_Reg spell_table[] = {
	{NULL, NULL}
};

static luaL_Reg spell_metatable[] = {
	{"get_x", entity_get_x},
	{"get_y", entity_get_y},
	{"get_w", entity_get_w},
	{"get_h", entity_get_h},
	{"set_x", entity_set_x},
	{"set_y", entity_set_y},
	{"get_speed", spell_get_speed},
	{"set_dx", spell_set_dx},
	{"set_dy", spell_set_dy},
	{"get_dx", spell_get_dx},
	{"get_dy", spell_get_dy},
	{"get_dis", spell_get_dis},
	{"get_max_dis", spell_get_max_dis},
	{"set_dis", spell_set_dis},
	{"get_owner", spell_get_owner},
	{"destroy", entity_destroy},
	{"move", spell_move},
	{"death", spell_death},
	{NULL, NULL}
};

void lua_init_spell(ScriptManager *scriptManager, lua_State *L) {
	luaW_register<Entity>(L, "Spell", spell_table, spell_metatable, spell_new);
}