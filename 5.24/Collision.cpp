#include "Collision.h"

#include "Environment.h"
#include "ResourceManager.h"

#include "Entity.h"
#include "PositionComponent.h"

bool collision(const SDL_Rect &a, const SDL_Rect &b) {
	if ((a.x + a.w) <= b.x) {
		return false;
	}
	if (a.x >= (b.x + b.w)) {
		return false;
	}
	if ((a.y + a.h) <= b.y) {
		return false;
	}
	if (a.y >= (b.y + b.h)) {
		return false;
	}
	return true;
}

bool spell_collisions(const SDL_Rect &spell_pos) {
	EntityManager::Entity_Map *objects = Environment::get().get_resource_manager()->get_entities(TYPE_OBJECT);
	PositionComponent *position = nullptr;
	for (auto it = objects->begin(); it != objects->end(); it++) {
		if (position = GetPosition(it->second)) {
			if (collision(spell_pos, position->rect)) {
				return true;
			}
		}
	}
	return false;
}