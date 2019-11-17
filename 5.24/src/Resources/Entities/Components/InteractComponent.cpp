#include "InteractComponent.h"

#include "Environment.h"
#include "_Lua.h"

#include <iostream>

InteractComponent::InteractComponent(Entity *entity_, std::string name_, std::string script_name_, std::string script_, std::string ui_info_) :
	Component	( entity_ ),
	name		( name_ ),
	script_name ( script_name_ ),
	script		( script_ ),
	ui_info		( ui_info_ ),
	special_id  ( -1 )
{
	Environment::get().get_lua()->load_script(script);
}

InteractComponent::InteractComponent(Entity *new_entity, const InteractComponent &rhs) :
	Component	( new_entity ),
	name		( rhs.name ),
	script_name ( rhs.script_name ),
	script		( rhs.script ),
	ui_info		( rhs.ui_info ),
	special_id  ( -1 )
{}

InteractComponent *InteractComponent::copy(Entity *new_entity) const {
	return new InteractComponent(new_entity, *this);
}

void InteractComponent::update() {}

void InteractComponent::interact() {
	lua_State *L = Environment::get().get_lua()->get_state();
	lua_getglobal(L, script_name.c_str());
	lua_getfield(L, -1, "interact");
	lua_remove(L, -2);
	luaW_push<InteractComponent>(L, this);
	lua_pcall(L, 1, 0, 0);
}