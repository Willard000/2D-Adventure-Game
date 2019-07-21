#include "Spell_Lua.h"

#include "SpellComponent.h"
#include "PositionComponent.h"
#include "SpriteComponent.h"

#include "Environment.h"
#include "Log.h"
#include "ResourceManager.h"
#include "Clock.h"

#include "Collision.h"

// spell_new
// not implemented
static SpellComponent *spell_new(lua_State *L) {
	return nullptr;
}

// Entity Spell:get_entity()
static int spell_get_entity(lua_State *L) {
	SpellComponent *spell = luaW_check<SpellComponent>(L, -1);
	luaW_push<Entity>(L, spell->entity);
	return 1;
}

// float Spell:get_speed()
static int spell_get_speed(lua_State *L) {
	SpellComponent *spell = luaW_check<SpellComponent>(L, -1);
	lua_pushnumber(L, spell->speed);
	return 1;
}

// void Spell:set_dx(float dx)
static int spell_set_dx(lua_State *L) {
	SpellComponent *spell = luaW_check<SpellComponent>(L, -2);
	float dx = (float)luaL_checknumber(L, -1);
	spell->dx = dx;
	return 0;
}

// void Spell:set_dy(float dy)
static int spell_set_dy(lua_State *L) {
	SpellComponent *spell = luaW_check<SpellComponent>(L, -2);
	float dy = (float)luaL_checknumber(L, -1);
	spell->dy = dy;
	return 0;
}

// float Spell:get_dx()
static int spell_get_dx(lua_State *L) {
	SpellComponent *spell = luaW_check<SpellComponent>(L, -1);
	lua_pushnumber(L, spell->dx);
	return 1;
}

// float Spell:get_dy()
static int spell_get_dy(lua_State *L) {
	SpellComponent *spell = luaW_check<SpellComponent>(L, -1);
	lua_pushnumber(L, spell->dy);
	return 1;
}

// void Spell:set_dis(float dis)
static int spell_set_dis(lua_State *L) {
	SpellComponent *spell = luaW_check<SpellComponent>(L, -2);
	float dis = (float)luaL_checknumber(L, -1);
	spell->dis = dis;
	return 0;
}

// float Spell:get_dis()
static int spell_get_dis(lua_State *L) {
	SpellComponent *spell = luaW_check<SpellComponent>(L, -1);
	lua_pushnumber(L, spell->dis);
	return 1;
}

// float Spell:get_max_dis()
static int spell_get_max_dis(lua_State *L) {
	SpellComponent *spell = luaW_check<SpellComponent>(L, -1);
	lua_pushnumber(L, spell->max_dis);
	return 1;
}

// Entity Spell:get_caster()
static int spell_get_caster(lua_State *L) {
	SpellComponent *spell = luaW_check<SpellComponent>(L, -1);
	luaW_push<Entity>(L, spell->caster);
	return 1;
}

// void Spell:move(float x, float y)
static int spell_move(lua_State *L) {
	SpellComponent *spell = luaW_check<SpellComponent>(L, -3);
	float x = (float)luaL_checknumber(L, -2);
	float y = (float)luaL_checknumber(L, -1);
	float xdis = float(x * Environment::get().get_clock()->get_time());
	float ydis = float(y * Environment::get().get_clock()->get_time());

	spell->dis += abs(xdis) + abs(ydis);

	if (PositionComponent *position = GetPosition(spell->entity)) {
		position->pos_x += xdis;
		position->pos_y += ydis;
		position->rect.x = (int)position->pos_x;
		position->rect.y = (int)position->pos_y;
		if (Environment::get().get_resource_manager()->get_map()->entity_collision(position->rect)) {
			spell->death();
		}
	}

	return 0;
}

// void Spell:death()
static int spell_death(lua_State *L) {
	SpellComponent *spell = luaW_check<SpellComponent>(L, -1);
	spell->death();
	return 0;
}

// void Spell:set_ani(int animation_type)
// 3 - IDLE
static int spell_set_ani(lua_State *L) {
	SpellComponent *spell = luaW_check<SpellComponent>(L, -2);
	int ani = (int)luaL_checknumber(L, -1);
	if (SpriteComponent *sprite = GetSprite(spell->entity)) {
		sprite->ani = ani;
	}
	return 0;
}

static luaL_Reg spell_table[] = {
	{NULL, NULL}
};

static luaL_Reg spell_metatable[] = {
	// func
	{"move", spell_move},
	{"death", spell_death},

	// set
	{"set_dx", spell_set_dx},
	{"set_dy", spell_set_dy},

	{"set_ani", spell_set_ani},

	{"get_set_dis", spell_set_dis},


	// get
	{"get_entity", spell_get_entity},
	{"get_caster", spell_get_caster},
	{"get_speed", spell_get_speed},

	{"get_dx", spell_get_dx},
	{"get_dy", spell_get_dy},

	{"get_dis", spell_get_dis},
	{"get_max_dis", spell_get_max_dis},

	{NULL, NULL}
};

void lua_init_spell(Lua *lua, lua_State *L) {
	luaW_register<SpellComponent>(L, "Spell", spell_table, spell_metatable, spell_new);
}