#include "PlayerComponent.h"

#include "Environment.h"
#include "ResourceManager.h"

#include "PositionComponent.h"

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