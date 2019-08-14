#include "PlayerComponent.h"

#include "Environment.h"
#include "ResourceManager.h"

#include "PositionComponent.h"

#include "Collision.h"

PlayerComponent::PlayerComponent(Entity *entity_, std::string name_) :
	Component		 ( entity_ ),
	name			 ( name_ )
{}

PlayerComponent::PlayerComponent(Entity *new_entity, const PlayerComponent &rhs) :
	Component		 ( new_entity ),
	name			 ( rhs.name )
{}

PlayerComponent *PlayerComponent::copy(Entity *new_entity) const {
	return new PlayerComponent(new_entity, *this);
}

PlayerComponent::~PlayerComponent() {
	for (auto &item : items) {
		delete item;
	}
}

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

	const auto entity_vec = Environment::get().get_resource_manager()->get_map()->get_entity_grid()->get_cells(position->rect);

	for (auto &vec : entity_vec) {
		for (auto &e : *vec) {
			const int &type = e->get_type();
			if (type == TYPE_OBJECT ||
				type == TYPE_ENEMY) {
				if (collision(position->rect, GetPosition(e)->rect))
					return true;
			}
		}
	}

	if (Environment::get().get_resource_manager()->get_map()->solid_collision(position->rect) ||
		Environment::get().get_resource_manager()->get_map()->bound_collision(position->rect))
		return true;

	return false;
}