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

#include "UIHandler.h"

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

#define SELECTION_XOFFSET 425
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

#define LEVEL_COLOR {150, 100, 200, 200}
#define EXP_COLOR {150, 100, 200, 200}
#define HEALTH_COLOR {225, 55, 55, 200}
#define MANA_COLOR {100, 185, 225, 200}
#define DAMAGE_COLOR {240, 160, 110, 200}
#define ARMOR_COLOR {210, 130, 200, 200}
#define HPS_COLOR {225, 55, 55, 200}
#define MPS_COLOR {100, 185, 225, 200}
#define DRAIN_COLOR {100, 185, 225, 200}
#define SPEED_COLOR {135, 210, 140, 200}
#define LUCK_COLOR {250, 170, 100, 200}

#define ENTITY_INFO_LABEL_XOFFSET 25
#define ENTITY_INFO_VAL_XOFFSET 350
#define ENTITY_INFO_YOFFSET 100
#define ENTITY_INFO_FT_SIZE 20
#define ENTITY_INFO_WRAP_LENGTH 100
#define ENTITY_INFO_NAME_COLOR {250, 250, 250, 200}

#define BUTTON_EQUIP_RECT {1300, 300, 100, 20}
#define BUTTON_USE_RECT {1300, 320, 100, 20}
#define BUTTON_DROP_RECT {1300, 340, 100, 20}
#define BUTTON_FT_SIZE 16
#define BUTTON_WRAP_LENGTH 1000
#define BUTTON_TEXT_COLOR {255, 255, 255, 200}
#define BUTTON_COLOR {0, 0, 0, 200}

#define SLOT_COLOR {0, 0, 0, 200}
#define SLOT_WEAPON_RECT {625, 284, ITEM_SIZE, ITEM_SIZE}
#define SLOT_HEAD_RECT {625, 200, ITEM_SIZE, ITEM_SIZE}
#define SLOT_CHEST_RECT {625, 242, ITEM_SIZE, ITEM_SIZE}
#define SLOT_RING_RECT {625, 326, ITEM_SIZE, ITEM_SIZE}

#define SLOT_OFFHAND_RECT {875, 284, ITEM_SIZE, ITEM_SIZE}
#define SLOT_GLOVES_RECT {875, 200, ITEM_SIZE, ITEM_SIZE}
#define SLOT_LEGS_RECT {875, 242, ITEM_SIZE, ITEM_SIZE}
#define SLOT_BOOTS_RECT {875, 326, ITEM_SIZE, ITEM_SIZE}

Inventory::Inventory(Entity *entity, std::vector<Entity *> *items, CombatComponent *stats) :
	_entity			( entity ),
	_items			( items ),
	_stats			( stats ),
	_selection		( {false, -1} )
{
	int width = int(0.8f * Environment::get().get_window()->get_width());
	int height = int(0.8f * Environment::get().get_window()->get_height());
	int x = int(width / 8);
	int y = int(height / 8);

	_background = { x, y, width, height };
	_items_background = { _background.w / 2 + ITEMS_XOFFSET, _background.h / 2 + ITEMS_YOFFSET, MAX_ROW_SIZE * ITEM_SIZE, (MAX_ITEMS / MAX_ROW_SIZE) * ITEM_SIZE };
	_selection_info.hide = true;

	setup_buttons();
	setup_slots();
}

Inventory::~Inventory() {
	for (auto &button : _buttons) {
		delete button;
	}
}

void Inventory::open() {
	set_entity_info();
	update_inventory_size();

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
		int mouse_x = input_manager->get_mouse_x();
		int mouse_y = input_manager->get_mouse_y();
		select_item(mouse_x, mouse_y);

		SDL_Rect mouse_rect = { mouse_x, mouse_y, 1, 1 };
		for (auto &button : _buttons) {
			if (collision(mouse_rect, button->get_rect())) {
				button->execute();
				break;
			}
		}
	}

	if (input_manager->is_key(SDL_SCANCODE_E)) {
		equip_item();
	}

	if (input_manager->is_key(SDL_SCANCODE_Q)) {
		drop_item();
	}

	if (input_manager->is_key(SDL_SCANCODE_F)) {
		_exit = true;
	}
}

void Inventory::update() {}

void Inventory::render() {
	Renderer *renderer = Environment::get().get_window()->get_renderer();

	renderer->clear();

	Environment::get().get_resource_manager()->render();

	renderer->draw_rect(_background, BACKGROUND_COLOR);
	renderer->draw_rect(_items_background, ITEMS_BACKGROUND_COLOR);

	render_entity();
	render_entity_info();
	render_slots();
	render_items();
	if (!_selection_info.hide) {
		render_item_info();
	}
	render_buttons();

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

void Inventory::render_slots() {
	Renderer *renderer = Environment::get().get_window()->get_renderer();

	for (auto &slot : _slots) {
		renderer->draw_rect(slot, SLOT_COLOR);
	}

	PlayerComponent *player = GetPlayer(_entity);
	for (unsigned int i = 0; i < TOTAL_SLOTS; ++i) {
		if (player->equipped_items[i] != nullptr) {
			renderer->render(Environment::get().get_resource_manager()->get_texture_info(player->equipped_items[i]), _slots[i], true);
		}
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

	if (_selection.index != -1) {
		SDL_Rect selection_rect;
		if (!_selection_info.hide) {
			selection_rect = { _background.w / 2 + SELECTION_XOFFSET, _background.h / 2 + SELECTION_YOFFSET, SELECTION_SIZE, SELECTION_SIZE };
			renderer->draw_rect(selection_rect, SELECTION_MAIN_COLOR);

			if (!_selection.equipped)
				renderer->render(Environment::get().get_resource_manager()->get_texture_info(_items->at(_selection.index)), selection_rect, true);
			else
				renderer->render(Environment::get().get_resource_manager()->get_texture_info(GetPlayer(_entity)->equipped_items[_selection.index]), selection_rect, true);
		}

		int selection_x = _selection.index % MAX_ROW_SIZE;
		int selection_y = _selection.index / MAX_ROW_SIZE;
		selection_rect = { selection_x * ITEM_SIZE + _items_background.x, selection_y * ITEM_SIZE + _items_background.y, ITEM_SIZE, ITEM_SIZE };


		if (!_selection.equipped)
			renderer->draw_rect(selection_rect, SELECTION_COLOR);
		else
			renderer->draw_rect(_slots[_selection.index], SELECTION_COLOR);
	}

	renderer->draw_text(&_inventory_size, true);
}

void Inventory::render_item_info() {
	if (_selection.index == -1) {
		return;
	}

	Renderer *renderer = Environment::get().get_window()->get_renderer();

	ItemComponent *item = nullptr;
	if (!_selection.equipped)
		item = GetItem(_items->at(_selection.index));
	else
		item = GetItem(GetPlayer(_entity)->equipped_items[_selection.index]);

	renderer->draw_text(&_selection_info.name, true);
	if (item->health > 0) {
		renderer->draw_text(&_selection_info.health, true);		renderer->draw_text(&_selection_info.health_val, true);
	}
	if (item->mana > 0) {
		renderer->draw_text(&_selection_info.mana, true);		renderer->draw_text(&_selection_info.mana_val, true);
	}
	if (item->damage > 0) {
		renderer->draw_text(&_selection_info.damage, true);		renderer->draw_text(&_selection_info.damage_val, true);
	}
	if (item->armor > 0) {
		renderer->draw_text(&_selection_info.armor, true);		renderer->draw_text(&_selection_info.armor_val, true);
	}
	if (item->hps > 0) {
		renderer->draw_text(&_selection_info.hps, true);		renderer->draw_text(&_selection_info.hps_val, true);
	}
	if (item->mps > 0) {
		renderer->draw_text(&_selection_info.mps, true);		renderer->draw_text(&_selection_info.mps_val, true);
	}
	if (item->drain > 0) {
		renderer->draw_text(&_selection_info.drain, true);		renderer->draw_text(&_selection_info.drain_val, true);
	}
	if (item->speed > 0) {
		renderer->draw_text(&_selection_info.speed, true);		renderer->draw_text(&_selection_info.speed_val, true);
	}
	if (item->luck > 0) {
		renderer->draw_text(&_selection_info.luck, true);		renderer->draw_text(&_selection_info.luck_val, true);
	}
}

void Inventory::render_entity_info() {
	Renderer *renderer = Environment::get().get_window()->get_renderer();

	renderer->draw_text(&_entity_info.name, true, DRAW_TEXT_NON_CENTER);
	renderer->draw_text(&_entity_info.level, true, DRAW_TEXT_NON_CENTER);		renderer->draw_text(&_entity_info.level_val, true);
	renderer->draw_text(&_entity_info.exp, true, DRAW_TEXT_NON_CENTER);			renderer->draw_text(&_entity_info.exp_val, true);
	renderer->draw_text(&_entity_info.health, true, DRAW_TEXT_NON_CENTER);		renderer->draw_text(&_entity_info.health_val, true);
	renderer->draw_text(&_entity_info.mana, true, DRAW_TEXT_NON_CENTER);		renderer->draw_text(&_entity_info.mana_val, true);
	renderer->draw_text(&_entity_info.damage, true, DRAW_TEXT_NON_CENTER);		renderer->draw_text(&_entity_info.damage_val, true);
	renderer->draw_text(&_entity_info.armor, true, DRAW_TEXT_NON_CENTER);		renderer->draw_text(&_entity_info.armor_val, true);
	renderer->draw_text(&_entity_info.hps, true, DRAW_TEXT_NON_CENTER);			renderer->draw_text(&_entity_info.hps_val, true);
	renderer->draw_text(&_entity_info.mps, true, DRAW_TEXT_NON_CENTER);			renderer->draw_text(&_entity_info.mps_val, true);
	renderer->draw_text(&_entity_info.drain, true, DRAW_TEXT_NON_CENTER);		renderer->draw_text(&_entity_info.drain_val, true);
	renderer->draw_text(&_entity_info.speed, true, DRAW_TEXT_NON_CENTER);		renderer->draw_text(&_entity_info.speed_val, true);
	renderer->draw_text(&_entity_info.luck, true, DRAW_TEXT_NON_CENTER);		renderer->draw_text(&_entity_info.luck_val, true);
}

void Inventory::render_buttons() {
	Renderer *renderer = Environment::get().get_window()->get_renderer();

	for (auto &button : _buttons) {
		renderer->draw_rect(button->get_rect(), button->get_color());
		renderer->draw_text(&button->get_text(), true);
	}
}

void Inventory::select_item(int mouse_x, int mouse_y) {
	SDL_Rect mouse_rect = { mouse_x, mouse_y, 1, 1 };
	for (unsigned int i = 0; i < TOTAL_SLOTS; ++i) {
		if (collision(mouse_rect, _slots[i])) {
			set_item_equipped(true);
			_selection.index = i;
			Entity *item = GetPlayer(_entity)->equipped_items[_selection.index];

			if (item) {
				ItemComponent *item_component = GetItem(item);
				if (item_component)
					set_item_info(item_component);
			}
			else
				_selection_info.hide = true;

			return;
		}
	}

	if (mouse_x < _items_background.x || mouse_x > _items_background.x + _items_background.w ||
		mouse_y < _items_background.y || mouse_y > _items_background.y + _items_background.h) {
		return;
	}

	int x = (mouse_x - _items_background.x) / ITEM_SIZE;
	int y = (mouse_y - _items_background.y) / ITEM_SIZE;
	set_item_equipped(false);
	_selection.index = x + y * MAX_ROW_SIZE;

	if (_selection.index >= (int)_items->size()) {
		_selection.index = -1;
		return;
	}

	set_item_info(GetItem(_items->at(_selection.index)));
}

void Inventory::set_item_info(ItemComponent *item) {
	if (!item) {
		_selection_info.hide = true;
		return;
	}

	int font_size = ITEM_INFO_FT_SIZE;
	int wrap_length = ITEM_INFO_WRAP_LENGTH;
	int label_x = (_background.w / 2 + SELECTION_XOFFSET + SELECTION_SIZE / 2) + ITEM_INFO_LABEL_XOFFSET;
	int val_x = (_background.w / 2 + SELECTION_XOFFSET + SELECTION_SIZE / 2) + ITEM_INFO_VAL_XOFFSET;
	int y = _background.h / 2 + SELECTION_YOFFSET + ITEM_INFO_YOFFSET;

	_selection_info.name = Text(item->name, ITEM_INFO_NAME_COLOR, font_size, wrap_length, _background.w / 2 + SELECTION_XOFFSET + SELECTION_SIZE /2, y);
	y += font_size + 2;

	if (item->health > 0) {
		_selection_info.health = Text("Health:", HEALTH_COLOR, font_size, wrap_length, label_x, y);
		_selection_info.health_val = Text(std::to_string(item->health), HEALTH_COLOR, font_size, wrap_length, val_x, y);
		y += font_size + 2;
	}

	if (item->hps > 0) {
		_selection_info.hps = Text("HPS:", HPS_COLOR, font_size, wrap_length, label_x, y);
		_selection_info.hps_val = Text(std::to_string(item->hps), HPS_COLOR, font_size, wrap_length, val_x, y);
		y += font_size + 2;
	}

	if (item->mana > 0) {
		_selection_info.mana = Text("Mana:", MANA_COLOR, font_size, wrap_length, label_x, y);
		_selection_info.mana_val = Text(std::to_string(item->mana), MANA_COLOR, font_size, wrap_length, val_x, y);
		y += font_size + 2;
	}

	if (item->mps > 0) {
		_selection_info.mps = Text("MPS:", MPS_COLOR, font_size, wrap_length, label_x, y);
		_selection_info.mps_val = Text(std::to_string(item->mps), MPS_COLOR, font_size, wrap_length, val_x, y);
		y += font_size + 2;
	}

	if (item->damage > 0) {
		_selection_info.damage = Text("Damage:", DAMAGE_COLOR, font_size, wrap_length, label_x, y);
		_selection_info.damage_val = Text(std::to_string(item->damage), DAMAGE_COLOR, font_size, wrap_length, val_x, y);
		y += font_size + 2;
	}

	if (item->armor > 0) {
		_selection_info.armor = Text("Armor:", ARMOR_COLOR, font_size, wrap_length, label_x, y);
		_selection_info.armor_val = Text(std::to_string(item->armor), ARMOR_COLOR, font_size, wrap_length, val_x, y);
		y += font_size + 2;
	}

	if (item->drain > 0) {
		_selection_info.drain = Text("Drain:", DRAIN_COLOR, font_size, wrap_length, label_x, y);
		_selection_info.drain_val = Text(std::to_string(item->drain), DRAIN_COLOR, font_size, wrap_length, val_x, y);
		y += font_size + 2;
	}

	if (item->speed > 0) {
		_selection_info.speed = Text("Speed:", SPEED_COLOR, font_size, wrap_length, label_x, y);
		_selection_info.speed_val = Text(std::to_string(item->speed), SPEED_COLOR, font_size, wrap_length, val_x, y);
		y += font_size + 2;
	}

	if (item->luck > 0) {
		_selection_info.luck = Text("Luck:", LUCK_COLOR, font_size, wrap_length, label_x, y);
		_selection_info.luck_val = Text(std::to_string(item->luck), LUCK_COLOR, font_size, wrap_length, val_x, y);
		y += font_size + 2;
	}
	_selection_info.hide = false;
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

	PositionComponent *position_info = GetPosition(_entity);
	if (!position_info) {
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
	_entity_info.level = Text("Level:", LEVEL_COLOR, font_size, wrap_length, label_x, y);
	_entity_info.level_val = Text(std::to_string(player_info->level), LEVEL_COLOR, font_size, wrap_length, val_x, y);

	y += font_size + 2;
	_entity_info.exp = Text("Exp:", EXP_COLOR, font_size, wrap_length, label_x, y);
	_entity_info.exp_val = Text(std::to_string(player_info->exp) + "/" + std::to_string(player_info->exp_to_level), EXP_COLOR, font_size, wrap_length, val_x, y);
	
	y += font_size + 2;
	_entity_info.health = Text("Health:", HEALTH_COLOR, font_size, wrap_length, label_x, y);
	_entity_info.health_val = Text(std::to_string(combat_info->health) + "/" + std::to_string(combat_info->max_health),
								HEALTH_COLOR, font_size, wrap_length, val_x, y);

	y += font_size + 2;
	_entity_info.hps = Text("HPS:", HPS_COLOR, font_size, wrap_length, label_x, y);
	_entity_info.hps_val = Text(std::to_string(combat_info->hps), HPS_COLOR, font_size, wrap_length, val_x, y);

	y += font_size + 2;
	_entity_info.mana = Text("Mana:", MANA_COLOR, font_size, wrap_length, label_x, y);
	_entity_info.mana_val = Text(std::to_string(combat_info->mana) + "/" + std::to_string(combat_info->max_mana),
								MANA_COLOR, font_size, wrap_length, val_x, y);
	y += font_size + 2;
	_entity_info.mps = Text("MPS:", MPS_COLOR, font_size, wrap_length, label_x, y);
	_entity_info.mps_val = Text(std::to_string(combat_info->mps), MPS_COLOR, font_size, wrap_length, val_x, y);

	y += font_size + 2;
	_entity_info.damage = Text("Damage:", DAMAGE_COLOR, font_size, wrap_length, label_x, y);
	_entity_info.damage_val = Text(std::to_string(combat_info->damage), DAMAGE_COLOR, font_size, wrap_length, val_x, y);

	y += font_size + 2;
	_entity_info.armor = Text("Armor:", ARMOR_COLOR, font_size, wrap_length, label_x, y);
	_entity_info.armor_val = Text(std::to_string(combat_info->armor), ARMOR_COLOR, font_size, wrap_length, val_x, y);

	y += font_size + 2;
	_entity_info.drain = Text("Drain:", DRAIN_COLOR, font_size, wrap_length, label_x, y);
	_entity_info.drain_val = Text(std::to_string(combat_info->drain), DRAIN_COLOR, font_size, wrap_length, val_x, y);

	y += font_size + 2;
	_entity_info.speed = Text("Speed:", SPEED_COLOR, font_size, wrap_length, label_x, y);
	_entity_info.speed_val = Text(std::to_string((int)position_info->speed), SPEED_COLOR, font_size, wrap_length, val_x, y);

	y += font_size + 2;
	_entity_info.luck = Text("Luck:", LUCK_COLOR, font_size, wrap_length, label_x, y);
	_entity_info.luck_val = Text(std::to_string(combat_info->luck), LUCK_COLOR, font_size, wrap_length, val_x, y);
}

void Inventory::set_item_equipped(bool is_equipped) {
	_selection.equipped = is_equipped;

	for (auto &button : _buttons) {
		if (is_equipped && button->get_text().get_text() == "Equip") {
			button->get_text().set_text("Unequip");
			return;
		}
		else if (!is_equipped && button->get_text().get_text() == "Unequip") {
			button->get_text().set_text("Equip");
			return;
		}
	}
}

void Inventory::update_inventory_size() {
	_inventory_size = Text(std::to_string(_items->size()) + "/" + std::to_string(MAX_ITEMS),
		INV_SIZE_COLOR, INV_SIZE_FT_SIZE, INV_SIZE_WRAP_LENGTH, _items_background.x + _items_background.w, _items_background.y + _items_background.h + INV_SIZE_FT_SIZE + 2);
}

void Inventory::setup_buttons() {
	_buttons.push_back(new UI::Button_Pressable(&UI::equip_item, "Equip", BUTTON_EQUIP_RECT, BUTTON_FT_SIZE, BUTTON_WRAP_LENGTH, BUTTON_TEXT_COLOR, BUTTON_COLOR));
	_buttons.push_back(new UI::Button_Pressable(&UI::drop_item, "Drop", BUTTON_DROP_RECT, BUTTON_FT_SIZE, BUTTON_WRAP_LENGTH, BUTTON_TEXT_COLOR, BUTTON_COLOR));
	_buttons.push_back(new UI::Button_Pressable(&UI::use_item, "Use", BUTTON_USE_RECT, BUTTON_FT_SIZE, BUTTON_WRAP_LENGTH, BUTTON_TEXT_COLOR, BUTTON_COLOR));
}

void Inventory::setup_slots() {
	_slots[SLOT_WEAPON] = SLOT_WEAPON_RECT;
	_slots[SLOT_OFFHAND] = SLOT_OFFHAND_RECT;
	_slots[SLOT_HEAD] = SLOT_HEAD_RECT;
	_slots[SLOT_CHEST] = SLOT_CHEST_RECT;
	_slots[SLOT_GLOVES] = SLOT_GLOVES_RECT;
	_slots[SLOT_LEGS] = SLOT_LEGS_RECT;
	_slots[SLOT_BOOTS] = SLOT_BOOTS_RECT;
	_slots[SLOT_RING] = SLOT_RING_RECT;
}

void Inventory::equip_item() {
	if (_selection.equipped) {
		unequip_item();
		return;
	}

	if (_selection.index == -1) {
		return;
	}

	ItemComponent *item = GetItem(_items->at(_selection.index));
	if (item->is_equipable == false) {
		return;
	}

	PlayerComponent *player = GetPlayer(_entity);
	if (!player) {
		return;
	}

	player->equip_item(_selection.index);

	_selection.index = -1;

	set_entity_info();
	update_inventory_size();
}

void Inventory::unequip_item() {
	if (!_selection.equipped) {
		equip_item();
		return;
	}

	if (_selection.index == -1) {
		return;
	}

	PlayerComponent *player = GetPlayer(_entity);
	player->unequip_item(_selection.index);

	_selection.index = -1;

	set_entity_info();
	update_inventory_size();
}

void Inventory::use_item() {
	// item lua script
}

void Inventory::drop_item() {
	if (_selection.equipped || _selection.index == -1) {
		return;
	}

	PositionComponent *player_position = GetPosition(_entity);
	PositionComponent *item_position = GetPosition(_items->at(_selection.index));

	item_position->set(player_position->pos_x, player_position->pos_y);

	Environment::get().get_resource_manager()->add_entity(_items->at(_selection.index));

	_items->erase(_items->begin() + _selection.index);

	_selection.index = -1;

	update_inventory_size();
}