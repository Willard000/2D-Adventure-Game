#include "CombatComponent.h"

#include "Entity.h"
#include "PositionComponent.h"

#include "Environment.h"
#include "Window.h"

#define HEALTH_BAR_HEIGHT 8
#define MANA_BAR_HEIGHT 6

#define HEALTH_BAR_COLOR {225, 55, 55, 150}
#define MANA_BAR_COLOR {100, 185, 225, 150}

CombatComponent::CombatComponent(Entity *entity_, int max_health_, int max_mana_, int damage_, int armor_, int combat_time) :
	Component		( entity_ ),
	max_health		( max_health_ ),
	health			( max_health_ ),
	max_mana		( max_mana_ ),
	mana			( max_mana_ ),
	damage			( damage_ ),
	armor			( armor_ ),
	in_combat       ( false ),
	combat_timer	( combat_time )
{}

CombatComponent::CombatComponent(Entity *new_entity, const CombatComponent &rhs) :
	Component		( new_entity ),
	max_health		( rhs.max_health ),
	health			( rhs.health ),
	max_mana		( rhs.max_mana ),
	mana			( rhs.mana ),
	damage			( rhs.damage ),
	armor			( rhs.armor ),
	in_combat       ( rhs.in_combat ),
	combat_timer    ( rhs.combat_timer )
{}

CombatComponent *CombatComponent::copy(Entity *new_entity) const {
	return new CombatComponent(new_entity, *this);
}

void CombatComponent::update() {
	if (in_combat) {
		in_combat = !combat_timer.update();
	}
}

const int CombatComponent::get_type() const {
	return COMPONENT_COMBAT;
}

void CombatComponent::apply_damage(int damage_) {
	start_combat();

	health -= damage_;

	if (health <= 0) {
		entity->destroy();
	}
}

void CombatComponent::draw_health() {
	if (!in_combat) {
		return;
	}

	PositionComponent *position = GetPosition(entity);
	if (!position) {
		return;
	}

	SDL_Rect health_bar = { position->rect.x - int(position->rect.w / 4), position->rect.y - position->rect.h, 0, HEALTH_BAR_HEIGHT };
	SDL_Rect mana_bar = { health_bar.x, health_bar.y + HEALTH_BAR_HEIGHT, 0, MANA_BAR_HEIGHT };

	float percent = 0;
	if (health > 0) {
		int width = position->rect.w + position->rect.w / 2;
		percent = (float)health / (float)max_health;
		health_bar.w = int(percent * width);

		if (mana > 0) {
			percent = (float)mana / (float)max_mana;
			mana_bar.w = int(percent * width);
		}
	}

	Environment::get().get_window()->get_renderer()->draw_rect(health_bar, HEALTH_BAR_COLOR, DRAW_RECT_CAMERA);
	Environment::get().get_window()->get_renderer()->draw_rect(mana_bar, MANA_BAR_COLOR, DRAW_RECT_CAMERA);
}

void CombatComponent::start_combat() {
	in_combat = true;
	combat_timer.reset();
}