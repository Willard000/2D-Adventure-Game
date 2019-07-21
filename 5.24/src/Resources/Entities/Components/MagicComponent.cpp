#include "MagicComponent.h"

#include "Environment.h"
#include "ResourceManager.h"

#include "Timer.h"

#include "PositionComponent.h"
#include "SpellComponent.h"

MagicComponent::MagicComponent(Entity *entity_, int main_spell_id_, int cast_speed_) :
	Component		( entity_ ),
	main_spell		( TYPE_SPELL, main_spell_id_ ),
	can_cast		( true ),
	cast_timer		( cast_speed_ )
{}

void MagicComponent::update() {
	if (cast_timer.update() && !can_cast) {
		can_cast = true;
	}
}

void MagicComponent::cast_main(double x_, double y_) {
	if (can_cast) {
		Entity *spell = new Entity(main_spell);
		SpellComponent *spell_comp = GetSpell(spell);
		spell_comp->caster = entity;
		spell_comp->cast();
		Environment::get().get_resource_manager()->add(spell);
		can_cast = false;
	}
}