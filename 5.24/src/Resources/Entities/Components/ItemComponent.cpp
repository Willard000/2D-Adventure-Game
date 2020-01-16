#include "ItemComponent.h"

#include "CombatComponent.h"

#include "Environment.h"
#include "_Lua.h"

ItemComponent::ItemComponent(
	Entity *entity_,
	int slot_,
	std::string name_,
	std::string info_,
	int health_,
	int mana_,
	int damage_,
	int armor_,
	int hps_,
	int mps_,
	int drain_,
	int speed_,
	int luck_,
	int duration_,
	int sell_value_,
	int buy_value_,
	bool is_equipable_,
	bool is_useable_,
	bool is_buffable_,
	bool is_stackable_,
	std::string script_name_,
	std::string script_,
	SDL_Color color_
) :
	Component		( entity_ ),
	slot			( slot_ ),
	name			( name_ ),
	info			( info_ ),
	health			( health_ ),
	mana			( mana_ ),
	damage			( damage_ ),
	armor			( armor_ ),
	hps				( hps_ ),
	mps				( mps_ ),
	drain			( drain_ ),
	speed			( speed_ ),
	luck			( luck_ ),
	duration		( duration_ ),
	sell_value		( sell_value_ ),
	buy_value		( buy_value_ ),
	stack_size		( 1 ),
	is_equipable	( is_equipable_ ),
	is_useable		( is_useable_),
	is_buffable     ( is_buffable_ ),
	is_stackable    ( is_stackable_ ),
	script_name     ( script_name_ ),
	script          ( script_ ),
	color           { color_ },
	destroy			( false )
{
	Environment::get().get_lua()->load_script(script);
}

ItemComponent::ItemComponent(Entity *new_entity, const ItemComponent &rhs) :
	Component		( new_entity ),
	slot			( rhs.slot ),
	name			( rhs.name ),
	info			( rhs.info ),
	health			( rhs.health ),
	mana			( rhs.mana ),
	damage			( rhs.damage ),
	armor			( rhs.armor ),
	hps				( rhs.hps ),
	mps				( rhs.mps ),
	drain			( rhs.drain ),
	speed			( rhs.speed ),
	luck			( rhs.luck ),
	duration		( rhs.duration ),
	sell_value		( rhs.sell_value ),
	buy_value		( rhs.buy_value ),
	stack_size		( rhs.stack_size ),
	is_equipable	( rhs.is_equipable ),
	is_useable		( rhs.is_useable ),
	is_buffable     ( rhs.is_buffable ),
	is_stackable	( rhs.is_stackable ),
	script_name     ( rhs.script_name ),
	script          ( rhs.script ),
	color			( rhs.color ),
	destroy         ( rhs.destroy )
{}

ItemComponent *ItemComponent::copy(Entity *new_entity) const {
	return new ItemComponent(new_entity, *this);
}

void ItemComponent::use(Entity *owner) {
	if (is_buffable) {
		if (CombatComponent *combat = GetCombat(owner)) {
			Buff_Info buff = { duration, health, mana, damage, armor, speed, luck, entity->get_type_id() };
			combat->add_buff(buff);
		}
	}

	if (script != "") {
		lua_State *L = Environment::get().get_lua()->get_state();
		lua_getglobal(L, script_name.c_str());
		lua_getfield(L, -1, "use");
		lua_remove(L, -2);
		luaW_push<ItemComponent>(L, this);
		lua_pcall(L, 1, 0, 0);
	}
}