#include "MagicComponent.h"

#include "Environment.h"
#include "ResourceManager.h"

#include "Timer.h"

#include "PositionComponent.h"
#include "SpellComponent.h"

MagicComponent::MagicComponent(Entity *entity_, int main_spell_id_, int secondary_spell_id_, int cast_speed_) :
	Component				( entity_ ),
	main_spell_id			( main_spell_id_ ),
	secondary_spell_id		( secondary_spell_id_ ),
	main_spell				( new Entity(TYPE_SPELL, main_spell_id_) ),
	secondary_spell			( new Entity(TYPE_SPELL, secondary_spell_id_) ),
	can_cast				( true ),
	cast_timer				( cast_speed_ )
{}

MagicComponent::MagicComponent(Entity *new_entity, const MagicComponent &rhs) :
	Component				( new_entity ),
	main_spell_id			( rhs.main_spell_id ),
	secondary_spell_id		( rhs.secondary_spell_id ),
	main_spell				( new Entity(TYPE_SPELL, rhs.main_spell_id) ),
	secondary_spell			( new Entity(TYPE_SPELL, rhs.secondary_spell_id) ),
	can_cast				( rhs.can_cast ),
	cast_timer				( rhs.cast_timer )
{}

MagicComponent *MagicComponent::copy(Entity *new_entity) const {
	return new MagicComponent(new_entity, *this);
}

MagicComponent::~MagicComponent() {
	delete main_spell;
	main_spell = nullptr;
	delete secondary_spell;
	secondary_spell = nullptr;
}

void MagicComponent::update() {
	if (cast_timer.update() && !can_cast) {
		can_cast = true;
	}
}

void MagicComponent::cast_main(float x_, float y_, float x2_, float y2_) {
	if (can_cast) {
		Entity *spell = new Entity(*main_spell);
		SpellComponent *spell_comp = GetSpell(spell);
		spell_comp->caster = entity;
		
		Combat_Info attacker_info;
		CombatComponent *combat_info = GetCombat(entity);
		if (combat_info) {
			attacker_info = { combat_info->damage, combat_info->armor, combat_info->drain, combat_info->luck };
		}
		if (combat_info->mana >= spell_comp->mana_cost) {
			combat_info->mana -= spell_comp->mana_cost;
			spell_comp->cast(x_, y_, x2_, y2_, attacker_info);
			Environment::get().get_resource_manager()->add_entity(spell);
			cast_timer.reset();
		}
		else {
			delete spell;
		}
		//can_cast = false;
	}
}

void MagicComponent::cast_secondary(float x_, float y_, float x2_, float y2_) {
	if (can_cast) {
		Entity *spell = new Entity(*secondary_spell);
		SpellComponent *spell_comp = GetSpell(spell);
		spell_comp->caster = entity;

		Combat_Info attacker_info;
		CombatComponent *combat_info = GetCombat(entity);
		if (combat_info) {
			attacker_info = { combat_info->damage, combat_info->armor, combat_info->drain, combat_info->luck };
		}
		if (combat_info->mana >= spell_comp->mana_cost) {
			combat_info->mana -= spell_comp->mana_cost;
			spell_comp->cast(x_, y_, x2_, y2_, attacker_info);
			Environment::get().get_resource_manager()->add_entity(spell);
			cast_timer.reset();
		}
		else {
			delete spell;
		}
		//can_cast = false;
	}
}

// ignores mana, cast time, and uses any spell id, used for making speical abilities in lua
void MagicComponent::cast_special(float x_, float y_, float x2_, float y2_, int spell_id) {
	Entity *spell = new Entity(TYPE_SPELL, spell_id);
	SpellComponent *spell_comp = GetSpell(spell);
	spell_comp->caster = entity;

	Combat_Info attacker_info;
	CombatComponent *combat_info = GetCombat(entity);
	if (combat_info) {
		attacker_info = { combat_info->damage, combat_info->armor, combat_info->drain, combat_info->luck };
	}
	spell_comp->cast(x_, y_, x2_, y2_, attacker_info);
	Environment::get().get_resource_manager()->add_entity(spell);
	cast_timer.reset();
}

void MagicComponent::set_main(int id) {
	delete main_spell;
	main_spell = new Entity(TYPE_SPELL, id);
	main_spell_id = id;
}

void MagicComponent::set_secondary(int id) {
	delete secondary_spell;
	secondary_spell = new Entity(TYPE_SPELL, id);
	secondary_spell_id = id;
}