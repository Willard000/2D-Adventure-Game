#include "EnemyComponent.h"

#include "Environment.h"
#include "_Lua.h"

EnemyComponent::EnemyComponent(Entity *entity_, std::string name_, std::string script_) :
	Component		( entity_ ),
	name			( name_ ),
	script			( script_ )
{
	Environment::get().get_lua()->load_script(script);
}

void EnemyComponent::update() {
	lua_State *L = Environment::get().get_lua()->get_state();
	lua_getglobal(L, name.c_str());
	lua_getfield(L, -1, "update");
	lua_remove(L, -2);
	luaW_push<EnemyComponent>(L, this);
	lua_pcall(L, 1, 0, 0);
}