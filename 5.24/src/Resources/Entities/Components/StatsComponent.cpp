#include "StatsComponent.h"

#include "Entity.h"

StatsComponent::StatsComponent(Entity *entity_, int max_health_, int max_mana_, int damage_, int armor_) :
	Component		( entity_ ),
	max_health		( max_health_ ),
	health			( max_health_ ),
	max_mana		( max_mana_ ),
	mana			( max_mana_ ),
	damage			( damage_ ),
	armor			( armor_ )
{}

StatsComponent::StatsComponent(Entity *new_entity, const StatsComponent &rhs) :
	Component		( new_entity ),
	max_health		( rhs.max_health ),
	health			( rhs.health ),
	max_mana		( rhs.max_mana ),
	mana			( rhs.mana ),
	damage			( rhs.damage ),
	armor			( rhs.armor )
{}

StatsComponent *StatsComponent::copy(Entity *new_entity) const {
	return new StatsComponent(new_entity, *this);
}

void StatsComponent::update() {}

const int StatsComponent::get_type() const {
	return COMPONENT_STATS;
}

void StatsComponent::apply_damage(int damage_) {
	health -= damage_;


	if (health <= 0) {
		entity->destroy();
	}
}