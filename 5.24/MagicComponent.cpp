#include "MagicComponent.h"

#include "Environment.h"
#include "ResourceManager.h"

#include "Timer.h"

#include "PositionComponent.h"
#include "SpellComponent.h"

MagicComponent::MagicComponent(Entity *entity_, int main_spell_id_, int cast_speed_) :
	Component		( entity_ ),
	main_spell		( TYPE_SPELL, main_spell_id_ ),
	cast_speed		( cast_speed_ ),
	can_cast		( true )
{}

void MagicComponent::update() {
	static Timer cast_time(cast_speed);
	if (cast_time.update() && !can_cast) {
		can_cast = true;
	}
}

void MagicComponent::cast_main(double x_, double y_) {
	if (can_cast) {
		Entity *spell = new Entity(main_spell);
		SpellComponent *spell_comp = GetSpell(spell); 
		PositionComponent *spell_position = GetPosition(spell);
		PositionComponent *entity_position = GetPosition(entity);
		if (spell_position && entity_position) {
			spell_position->set(
				entity_position->pos_x + (entity_position->rect.w / 2) - (spell_position->rect.w / 2),
				entity_position->pos_y + (entity_position->rect.h / 2) - (spell_position->rect.h / 2)
			);
			spell_comp->cast(x_, y_);
			Environment::get().get_resource_manager()->add(spell);
			can_cast = false;
		}
	}
}