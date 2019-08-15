#include "InputHandler.h"

#include "Environment.h"
#include "ResourceManager.h"
#include "Window.h"

#include "PositionComponent.h"
#include "MagicComponent.h"
#include "PlayerComponent.h"

void move_entity(Entity *entity, int dir) {
	if (PositionComponent *position = GetPosition(entity)) {
		position->move(dir);
	}
}

void move_camera(int dir) {
	Environment::get().get_window()->get_camera()->move(dir);
}

void toggle_camera() {
	Environment::get().get_window()->get_camera()->toggle();
	if (Environment::get().get_window()->get_camera()->get_locked()) {
		Environment::get().get_window()->get_camera()->set_scale(1.0f);
	}
}

void cast_spell(float x, float y) {
	Camera *camera = Environment::get().get_window()->get_camera();
	x = (x / camera->get_scale()) + camera->get_x();
	y = (y / camera->get_scale()) + camera->get_y();

	MagicComponent *magic = GetMagic(Environment::get().get_resource_manager()->get_player());
	if (magic) {
		magic->cast_main(x, y);
	}
}

void pickup_item() {
	Entity *player = Environment::get().get_resource_manager()->get_player();
	PositionComponent *player_position = GetPosition(player);
	PlayerComponent *player_component = GetPlayer(player);
	if (player_component->items.size() == MAX_ITEMS) {
		return;
	}
	
	auto items = Environment::get().get_resource_manager()->get_entities(TYPE_ITEM);
	for (auto it = items->begin(); it != items->end(); ++it) {
		if (PositionComponent *item_position = GetPosition(it->second)) {
			if (collision(player_position->rect, item_position->rect)) {
				player_component->items.push_back(it->second);
				items->erase(it);
				return;
			}
		}
	}
}