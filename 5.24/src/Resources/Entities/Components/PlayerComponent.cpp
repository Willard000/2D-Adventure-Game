#include "PlayerComponent.h"

#include "Environment.h"
#include "ResourceManager.h"

#include "PositionComponent.h"

#include "Collision.h"

PlayerComponent::PlayerComponent(Entity *entity_) :
	Component		 ( entity_ )
{}

void PlayerComponent::update() {
	// WARP
	if (PositionComponent *position = GetPosition(entity)) {
		Map::Warp *warp = Environment::get().get_resource_manager()->get_map()->warp_collision(position->rect);
		if (warp != nullptr) {
			position->set(float(warp->to.x + (warp->to.w / 2) - (position->rect.w / 2)), float(warp->to.y + (warp->to.h / 2) - (position->rect.h / 2)));
			if (warp->to_id != Environment::get().get_resource_manager()->get_map()->get_id())
				Environment::get().get_resource_manager()->load_map(warp->to_id);
		}
	}
}

bool PlayerComponent::is_collision() {
	PositionComponent *position = GetPosition(entity);
	if (!position)
		return false;

	std::vector<Entity *> *entities = Environment::get().get_resource_manager()->get_map()->get_entity_grid()->get_objs(position->rect);
	if (!entities)
		return false;

	for (auto it = entities->begin(); it != entities->end(); ++it) {
		const int &type = (*it)->get_type();
		if ( type == TYPE_OBJECT ||
			 type == TYPE_ENEMY) {
			if (collision(position->rect, GetPosition((*it))->rect))
				return true;
		}
	}

	return false;
}