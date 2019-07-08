#include "Player.h"

#include "Component.h"
#include "PositionComponent.h"

#include "Environment.h"
#include "ResourceManager.h"

void Player::update() {
	Entity::update();


	// WARP
	if (PositionComponent *position = GetPosition(this)) {
		Map::Warp *warp = Environment::get().get_resource_manager()->get_map()->warp_collision(position->rect);
		if (warp != nullptr) {
			position->set(warp->to.x + (warp->to.w / 2) - (position->rect.w / 2), warp->to.y + (warp->to.h / 2) - (position->rect.h / 2));
			Environment::get().get_resource_manager()->load_map(warp->to_id);
		}
	}
}