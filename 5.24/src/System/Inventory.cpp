#include "Inventory.h"

#include "Environment.h"
#include "InputManager.h"
#include "Window.h"
#include "ResourceManager.h"
#include "Clock.h"
#include "TextureManager.h"

#include "PlayerComponent.h"
#include "PositionComponent.h"
#include "SpriteComponent.h"
#include "ItemComponent.h"
#include "EnemyComponent.h"

#define BACKGROUND_COLOR {0, 0, 0, 200}
#define ENTITY_CENTER_XOFFSET 64
#define ENTITY_CENTER_YOFFSET -128
#define ENTITY_CENTER_WIDTH 128
#define ENTITY_CENTER_HEIGHT 128

#define ITEMS_BACKGROUND_COLOR {0, 0, 0, 100}
#define ITEMS_XOFFSET 125
#define ITEMS_YOFFSET 250
#define ITEM_SIZE 32
#define MAX_ROW_SIZE 20
#define MAX_ITEMS 100

#define SELECTION_XOFFSET 400
#define SELECTION_YOFFSET -200
#define SELECTION_SIZE 86
#define SELECTION_MAIN_COLOR {0, 0, 0, 100}
#define SELECTION_COLOR {100, 0, 0, 100}

#define INV_SIZE_COLOR {255, 255, 255, 100}
#define INV_SIZE_FT_SIZE 14
#define INV_SIZE_WRAP_LENGTH 1000

#define ITEM_INFO_LABEL_XOFFSET -100
#define ITEM_INFO_VAL_XOFFSET 100
#define ITEM_INFO_YOFFSET 100
#define ITEM_INFO_FT_SIZE 20
#define ITEM_INFO_WRAP_LENGTH 1000
#define ITEM_INFO_NAME_COLOR {255, 0, 0, 200}

#define HEALTH_COLOR {225, 55, 55, 200}
#define MANA_COLOR {100, 185, 225, 200}
#define DAMAGE_COLOR {150, 0, 100, 200}
#define ARMOR_COLOR {100, 0, 150, 200}
#define HPS_COLOR {225, 55, 55, 200}
#define MPS_COLOR {100, 185, 225, 200}

#define ENTITY_INFO_LABEL_XOFFSET 50
#define ENTITY_INFO_VAL_XOFFSET 350
#define ENTITY_INFO_YOFFSET 100
#define ENTITY_INFO_FT_SIZE 20
#define ENTITY_INFO_WRAP_LENGTH 100
#define ENTITY_INFO_NAME_COLOR {255, 0, 0, 255}

Inventory::Inventory(Entity *entity, std::vector<Entity *> *items, CombatComponent *stats) :
	_entity			( entity ),
	_items			( items ),
	_stats			( stats ),
	_selection		( -1 )
{
	int width = int(0.8f * Environment::get().get_window()->get_width());
	int height = int(0.8f * Environment::get().get_window()->get_height());
	int x = int(width / 8);
	int y = int(height / 8);

	_background = { x, y, width, height };
	_items_background = { _background.w / 2 + ITEMS_XOFFSET, _background.h / 2 + ITEMS_YOFFSET, MAX_ROW_SIZE * ITEM_SIZE, (MAX_ITEMS / MAX_ROW_SIZE) * ITEM_SIZE };

	set_entity_info();
	update_inventory_size();
}

void Inventory::open() {
	_exit = false;
	while (!_exit) {
		input();
		update();
		render();

		if (Environment::get().get_clock()->update()) {
			std::string title = "Inventory      Map: " + std::to_string(Environment::get().get_resource_manager()->get_map()->get_id()) + "     " +
				Environment::get().get_clock()->get_display_time() + "    " + std::to_string(Environment::get().get_clock()->get_fms())
				+ " spells: " + std::to_string(Environment::get().get_resource_manager()->get_entities(TYPE_SPELL)->size());
			Environment::get().get_window()->set_title(title);
		}
	}
}

void Inventory::input() {
	InputManager *input_manager = Environment::get().get_input_manager();

	_exit = !input_manager->get();

	if (input_manager->is_mouse(SDL_BUTTON_LEFT)) {
		select_item(input_manager->get_mouse_x(), input_manager->get_mouse_y());
	}
}

void Inventory::update() {

}

void Inventory::render() {
	Renderer *renderer = Environment::get().get_window()->get_renderer();

	renderer->clear();

	Environment::get().get_resource_manager()->render();

	renderer->draw_rect(_background, BACKGROUND_COLOR);
	renderer->draw_rect(_items_background, ITEMS_BACKGROUND_COLOR);

	render_entity();
	render_entity_info();
	render_items();
	render_item_info();

	renderer->render();
}

void Inventory::render_entity() {
	Renderer *renderer = Environment::get().get_window()->get_renderer();

	SpriteComponent *sprite = GetSprite(_entity);

	SDL_Rect center_rect = { (_background.w / 2) + ENTITY_CENTER_XOFFSET, (_background.h / 2) + ENTITY_CENTER_YOFFSET, ENTITY_CENTER_WIDTH, ENTITY_CENTER_HEIGHT };

	if (sprite) {
		renderer->render(Environment::get().get_resource_manager()->get_sprite_info(_entity), sprite, center_rect, true);
	}
	else {
		renderer->render(Environment::get().get_resource_manager()->get_texture_info(_entity), center_rect);
	}
}

void Inventory::render_items() {
	Renderer *renderer = Environment::get().get_window()->get_renderer();

	int x = _items_background.x;
	int y = _items_background.y;
	for (unsigned int i = 0; i < _items->size(); ++i) {
		if (i % MAX_ROW_SIZE == 0 && i != 0) {
			y += ITEM_SIZE;
			x = _items_background.x;
		}
		else if (i != 0) {
			x += ITEM_SIZE;
		}
		SDL_Rect item_rect = {x, y, ITEM_SIZE, ITEM_SIZE};
		renderer->render(Environment::get().get_resource_manager()->get_texture_info(_items->at(i)), item_rect, true);
	}

	if (_selection != -1) {
		SDL_Rect selection_rect = { _background.w / 2 + SELECTION_XOFFSET, _background.h / 2 + SELECTION_YOFFSET, SELECTION_SIZE, SELECTION_SIZE };
		renderer->draw_rect(selection_rect, SELECTION_MAIN_COLOR);
		renderer->render(Environment::get().get_resource_manager()->get_texture_info(_items->at(_selection)), selection_rect, true);

		int selection_x = _selection % MAX_ROW_SIZE;
		int selection_y = _selection / MAX_ROW_SIZE;
		selection_rect = { selection_x * ITEM_SIZE + _items_background.x, selection_y * ITEM_SIZE + _items_background.y, ITEM_SIZE, ITEM_SIZE };
		renderer->draw_rect(selection_rect, SELECTION_COLOR);
	}

	renderer->draw_text(&_inventory_size, true);
}

void Inventory::render_item_info() {
	if (_selection == -1) {
		return;
	}

	Renderer *renderer = Environment::get().get_window()->get_renderer();

	renderer->draw_text(&_selection_info.name, true);
	renderer->draw_text(&_selection_info.health, true);
	renderer->draw_text(&_selection_info.health_val, true);
	renderer->draw_text(&_selection_info.mana, true);
	renderer->draw_text(&_selection_info.mana_val, true);
	renderer->draw_text(&_selection_info.damage, true);
	renderer->draw_text(&_selection_info.damage_val, true);
	renderer->draw_text(&_selection_info.armor, true);
	renderer->draw_text(&_selection_info.armor_val, true);
	renderer->draw_text(&_selection_info.hps, true);
	renderer->draw_text(&_selection_info.hps_val, true);
	renderer->draw_text(&_selection_info.mps, true);
	renderer->draw_text(&_selection_info.mps_val, true);
}

void Inventory::render_entity_info() {
	Renderer *renderer = Environment::get().get_window()->get_renderer();

	renderer->draw_text(&_entity_info.name, true);
	renderer->draw_text(&_entity_info.health, true);
	renderer->draw_text(&_entity_info.health_val, true);
	renderer->draw_text(&_entity_info.mana, true);
	renderer->draw_text(&_entity_info.mana_val, true);
	renderer->draw_text(&_entity_info.damage, true);
	renderer->draw_text(&_entity_info.damage_val, true);
	renderer->draw_text(&_entity_info.armor, true);
	renderer->draw_text(&_entity_info.armor_val, true);
	renderer->draw_text(&_entity_info.hps, true);
	renderer->draw_text(&_entity_info.hps_val, true);
	renderer->draw_text(&_entity_info.mps, true);
	renderer->draw_text(&_entity_info.mps_val, true);
}

void Inventory::select_item(int mouse_x, int mouse_y) {
	if (mouse_x < _items_background.x || mouse_x > _items_background.x + _items_background.w ||
		mouse_y < _items_background.y || mouse_y > _items_background.y + _items_background.h) {
		return;
	}

	int x = (mouse_x - _items_background.x) / ITEM_SIZE;
	int y = (mouse_y - _items_background.y) / ITEM_SIZE;
	_selection = x + y * MAX_ROW_SIZE;

	set_item_info(GetItem(_items->at(_selection)));
}

void Inventory::set_item_info(ItemComponent *item) {
	int font_size = ITEM_INFO_FT_SIZE;
	int wrap_length = ITEM_INFO_WRAP_LENGTH;
	int label_x = (_background.w / 2 + SELECTION_XOFFSET + SELECTION_SIZE / 2) + ITEM_INFO_LABEL_XOFFSET;
	int val_x = (_background.w / 2 + SELECTION_XOFFSET + SELECTION_SIZE / 2) + ITEM_INFO_VAL_XOFFSET;
	int y = _background.h / 2 + SELECTION_YOFFSET + ITEM_INFO_YOFFSET;

	_selection_info.name = Text(item->name, ITEM_INFO_NAME_COLOR, font_size, wrap_length, _background.w / 2 + SELECTION_XOFFSET + SELECTION_SIZE /2, y);
	y += font_size + 2;
	_selection_info.health = Text("Health: ", HEALTH_COLOR, font_size, wrap_length, label_x, y);
	_selection_info.health_val = Text(std::to_string(item->health), HEALTH_COLOR, font_size, wrap_length, val_x, y);
	y += font_size + 2;
	_selection_info.mana = Text("Mana: ", MANA_COLOR, font_size, wrap_length, label_x, y);
	_selection_info.mana_val = Text(std::to_string(item->mana), MANA_COLOR, font_size, wrap_length, val_x, y);
	y += font_size + 2;
	_selection_info.damage = Text("Damage: ", DAMAGE_COLOR, font_size, wrap_length, label_x, y);
	_selection_info.damage_val = Text(std::to_string(item->damage), DAMAGE_COLOR, font_size, wrap_length, val_x, y);
	y += font_size + 2;
	_selection_info.armor = Text("Armor: ", ARMOR_COLOR, font_size, wrap_length, label_x, y);
	_selection_info.armor_val = Text(std::to_string(item->armor), ARMOR_COLOR, font_size, wrap_length, val_x, y);
	y += font_size + 2;
	_selection_info.hps = Text("HPS: ", HPS_COLOR, font_size, wrap_length, label_x, y);
	_selection_info.hps_val = Text(std::to_string(item->hps), HPS_COLOR, font_size, wrap_length, val_x, y);
	y += font_size + 2;
	_selection_info.mps = Text("MPS: ", MPS_COLOR, font_size, wrap_length, label_x, y);
	_selection_info.mps_val = Text(std::to_string(item->mps), MPS_COLOR, font_size, wrap_length, val_x, y);
}

void Inventory::set_entity_info() {
	int font_size = ENTITY_INFO_FT_SIZE;
	int wrap_length = ENTITY_INFO_WRAP_LENGTH;
	int label_x = _background.x + ENTITY_INFO_LABEL_XOFFSET;
	int val_x = _background.x + ENTITY_INFO_VAL_XOFFSET;
	int y = ENTITY_INFO_YOFFSET;

	CombatComponent *combat_info = GetCombat(_entity);
	if (!combat_info) {
		return;
	}

	PlayerComponent *player_info = GetPlayer(_entity);
	if (player_info) {
		_entity_info.name = Text(player_info->name, ENTITY_INFO_NAME_COLOR, font_size, wrap_length, label_x + (val_x - label_x) / 2, y);
	}
	else {
		EnemyComponent *enemy_info = GetEnemy(_entity);
		if (enemy_info) {
			_entity_info.name = Text(enemy_info->name, ENTITY_INFO_NAME_COLOR, font_size, wrap_length, label_x + (val_x - label_x) / 2, y);
		}
	}

	y += font_size + 2;

	_entity_info.health = Text("Health: ", HEALTH_COLOR, font_size, wrap_length, label_x, y);
	_entity_info.health_val = Text(std::to_string(combat_info->health) + "/" + std::to_string(combat_info->max_health),
								HEALTH_COLOR, font_size, wrap_length, val_x, y);
	y += font_size + 2;
	_entity_info.mana = Text("Mana: ", MANA_COLOR, font_size, wrap_length, label_x, y);
	_entity_info.mana_val = Text(std::to_string(combat_info->mana) + "/" + std::to_string(combat_info->max_mana),
								MANA_COLOR, font_size, wrap_length, val_x, y);
	y += font_size + 2;
	_entity_info.damage = Text("Damage: ", DAMAGE_COLOR, font_size, wrap_length, label_x, y);
	_entity_info.damage_val = Text(std::to_string(combat_info->damage), DAMAGE_COLOR, font_size, wrap_length, val_x, y);
	y += font_size + 2;
	_entity_info.armor = Text("Armor:", ARMOR_COLOR, font_size, wrap_length, label_x, y);
	_entity_info.armor_val = Text(std::to_string(combat_info->armor), ARMOR_COLOR, font_size, wrap_length, val_x, y);
	y += font_size + 2;
	_entity_info.hps = Text("HPS: ", HPS_COLOR, font_size, wrap_length, label_x, y);
	_entity_info.hps_val = Text(std::to_string(combat_info->hps), HPS_COLOR, font_size, wrap_length, val_x, y);
	y += font_size + 2;
	_entity_info.mps = Text("MPS: ", MPS_COLOR, font_size, wrap_length, label_x, y);
	_entity_info.mps_val = Text(std::to_string(combat_info->mps), MPS_COLOR, font_size, wrap_length, val_x, y);
}

void Inventory::update_inventory_size() {
	_inventory_size = Text(std::to_string(_items->size()) + "/" + std::to_string(MAX_ITEMS),
		INV_SIZE_COLOR, INV_SIZE_FT_SIZE, INV_SIZE_WRAP_LENGTH, _items_background.x + _items_background.w, _items_background.y + _items_background.h + INV_SIZE_FT_SIZE + 2);
}