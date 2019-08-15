#include "ItemComponent.h"

ItemComponent::ItemComponent(Entity *entity_, int slot_, std::string name_, int health_, int mana_, int damage_, int armor_, int hps_, int mps_, bool is_equipable_, bool is_useable_) :
	Component		( entity_ ),
	slot			( slot_ ),
	name			( name_ ),
	health			( health_ ),
	mana			( mana_ ),
	damage			( damage_ ),
	armor			( armor_ ),
	hps				( hps_ ),
	mps				( mps_ ),
	is_equipable	( is_equipable_ ),
	is_useable		( is_useable_)
{}

ItemComponent::ItemComponent(Entity *new_entity, const ItemComponent &rhs) :
	Component		( new_entity ),
	slot			( rhs.slot ),
	name			( rhs.name ),
	health			( rhs.health ),
	mana			( rhs.mana ),
	damage			( rhs.damage ),
	armor			( rhs.armor ),
	hps				( rhs.hps ),
	mps				( rhs.mps ),
	is_equipable	( rhs.is_equipable ),
	is_useable		( rhs.is_useable )
{}

ItemComponent *ItemComponent::copy(Entity *new_entity) const {
	return new ItemComponent(new_entity, *this);
}