#include "InteractComponent.h"

#include "Environment.h"
#include "_Lua.h"
#include "Window.h"
#include "ResourceManager.h"


#include "Collision.h"

#define INTERACT_TEXT_COLOR {255, 255, 255, 225}
#define INTERACT_TEXT_FTSIZE 18
#define INTERACT_TEXT_WRAP_LENGTH 1000

#define INTERACT_RANGE_W 96  // 3x3 tile range
#define INTERACT_RANGE_H 96

// a's range is INTERACT_RANGE
// b's range is its size
bool interact_in_range(const SDL_Rect &a, const SDL_Rect &b) {
	SDL_Rect interact_range = {
		(a.x + a.w / 2) - INTERACT_RANGE_W / 2,
		(a.y + a.h / 2) - INTERACT_RANGE_H / 2,
		INTERACT_RANGE_W,
		INTERACT_RANGE_H
	};

	return collision(interact_range, b);
}

InteractComponent::InteractComponent(Entity *entity_, std::string name_, std::string script_name_, std::string script_, std::string ui_info_) :
	Component	( entity_ ),
	name		( name_ ),
	script_name ( script_name_ ),
	script		( script_ ),
	ui_info		( ui_info_ ),
	info_text   ( ui_info_, INTERACT_TEXT_COLOR, INTERACT_TEXT_FTSIZE, INTERACT_TEXT_WRAP_LENGTH, 0, 0)
{
	Environment::get().get_lua()->load_script(script);
}

InteractComponent::InteractComponent(Entity *new_entity, const InteractComponent &rhs) :
	Component	( new_entity ),
	name		( rhs.name ),
	script_name ( rhs.script_name ),
	script		( rhs.script ),
	ui_info		( rhs.ui_info ),
	info_text   ( rhs.info_text )
{}

InteractComponent *InteractComponent::copy(Entity *new_entity) const {
	return new InteractComponent(new_entity, *this);
}

void InteractComponent::update() {
	display_info();
}

void InteractComponent::interact() {
	lua_State *L = Environment::get().get_lua()->get_state();
	lua_getglobal(L, script_name.c_str());
	lua_getfield(L, -1, "interact");
	lua_remove(L, -2);
	luaW_push<InteractComponent>(L, this);
	lua_pcall(L, 1, 0, 0);
}

void InteractComponent::display_info() {
	PositionComponent *player_position = GetPosition(Environment::get().get_resource_manager()->get_player());
	PositionComponent *object_position = GetPosition(entity);

	if (interact_in_range(player_position->rect, object_position->rect)) {
		info_text.set_pos(object_position->rect.x - object_position->rect.w / 2, object_position->rect.y + object_position->rect.h);
		Environment::get().get_resource_manager()->add_text(&info_text);
	}
}