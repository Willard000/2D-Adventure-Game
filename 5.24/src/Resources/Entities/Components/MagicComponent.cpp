#include "MagicComponent.h"

#include "Environment.h"
#include "ResourceManager.h"

#include "Timer.h"

#include "PositionComponent.h"
#include "SpellComponent.h"

MagicComponent::MagicComponent(Entity *entity_, int main_spell_id_, int cast_speed_) :
	Component		( entity_ ),
	main_spell_id   ( main_spell_id_ ),
	main_spell		( TYPE_SPELL, main_spell_id_ ),
	can_cast		( true ),
	cast_timer		( cast_speed_ )
{}

MagicComponent::MagicComponent(Entity *new_entity, const MagicComponent &rhs) :
	Component		( new_entity ),
	main_spell_id   ( rhs.main_spell_id ),
	main_spell		( rhs.main_spell.get_type(), rhs.main_spell_id ),
	can_cast		( rhs.can_cast ),
	cast_timer		( rhs.cast_timer )
{}

MagicComponent *MagicComponent::copy(Entity *new_entity) const {
	return new MagicComponent(new_entity, *this);
}

void MagicComponent::update() {
	if (cast_timer.update() && !can_cast) {
		can_cast = true;
	}
}

void MagicComponent::cast_main(float x_, float y_) {
	if (can_cast) {
		Entity *spell = new Entity(main_spell);
		SpellComponent *spell_comp = GetSpell(spell);
		spell_comp->caster = entity;
		spell_comp->cast(x_, y_);
		Environment::get().get_resource_manager()->add_entity(spell);
		can_cast = false;
	}
}