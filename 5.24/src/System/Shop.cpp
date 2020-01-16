#include "Shop.h"

#include "Environment.h"
#include "InputManager.h"
#include "Window.h"
#include "Clock.h"
#include "ResourceManager.h"
#include "PlayerComponent.h"

#define BACKGROUND_COLOR {0, 0, 0, 200}

#define ITEMS_XOFFSET -450
#define ITEMS_YOFFSET 200
#define MAX_ROW_SIZE 15

#define SHOP_ITEMS_XOFFSET 285
#define SHOP_ITEMS_YOFFSET 200

#define ITEM_SIZE 32

#define GOLD_XOFFSET 20

#define INV_SIZE_COLOR {255, 255, 255, 100}
#define INV_SIZE_FT_SIZE 14
#define INV_SIZE_WRAP_LENGTH 1000

#define SELECTION_XOFFSET 110
#define SELECTION_YOFFSET -200
#define SELECTION_SIZE 86
#define SELECTION_MAIN_COLOR {0, 0, 0, 100}
#define SELECTION_COLOR {100, 0, 0, 100}

#define ITEM_INFO_LABEL_XOFFSET -100
#define ITEM_INFO_VAL_XOFFSET 100
#define ITEM_INFO_YOFFSET 100
#define ITEM_INFO_FT_SIZE 20
#define ITEM_INFO_WRAP_LENGTH 1000
#define ITEM_INFO_COLOR {0, 150, 150, 200}

#define ENTITY_LEFT_XOFFSET -275
#define ENTITY_LEFT_YOFFSET -100
#define ENTITY_LEFT_WIDTH 128
#define ENTITY_LEFT_HEIGHT 128
#define ENTITY_RIGHT_XOFFSET 450
#define ENTITY_RIGHT_YOFFSET -150
#define ENTITY_RIGHT_WIDTH 128
#define ENTITY_RIGHT_HEIGHT 128

#define BUTTON_SELL_RECT_W 100
#define BUTTON_SELL_RECT_H 20
#define BUTTON_FT_SIZE 16
#define BUTTON_WRAP_LENGTH 1000
#define BUTTON_TEXT_COLOR {255, 255, 255, 200}
#define BUTTON_COLOR {0, 0, 0, 200}

Shop::Shop(Entity *player, std::vector<Entity *> *inventory, Entity *shop_owner, std::vector<int> *shop) :
	_player			( player ),
	_inventory      ( inventory ),
	_shop_owner     ( shop_owner ),
	_shop           ( shop ),
	_shop_item	    ( nullptr ),
	_exit			( false )
{
	int width = int(0.8f * Environment::get().get_window()->get_width());
	int height = int(0.8f * Environment::get().get_window()->get_height());
	int x = int(width / 8);
	int y = int(height / 8);

	_background = { x, y, width, height };
	_items_background = { _background.w / 2 + ITEMS_XOFFSET, _background.h / 2 + ITEMS_YOFFSET, MAX_ROW_SIZE * ITEM_SIZE, ((MAX_ITEMS / MAX_ROW_SIZE) + 1) * ITEM_SIZE };
	_shop_items_background = { _background.w / 2 + SHOP_ITEMS_XOFFSET, _background.h / 2 + SHOP_ITEMS_YOFFSET, MAX_ROW_SIZE * ITEM_SIZE, ((MAX_ITEMS / MAX_ROW_SIZE) + 1) * ITEM_SIZE };

	SDL_Rect button_rect = { _items_background.x, _items_background.y - BUTTON_SELL_RECT_H - 2, BUTTON_SELL_RECT_W, BUTTON_SELL_RECT_H };
	_sell_button = new UI::Button_Pressable(nullptr, "Sell", button_rect, BUTTON_FT_SIZE, BUTTON_WRAP_LENGTH, BUTTON_TEXT_COLOR, BUTTON_COLOR);
	button_rect.x = _shop_items_background.x;
	button_rect.y = _shop_items_background.y - BUTTON_SELL_RECT_H - 2;
	_buy_button = new UI::Button_Pressable(nullptr, "Buy", button_rect, BUTTON_FT_SIZE, BUTTON_WRAP_LENGTH, BUTTON_TEXT_COLOR, BUTTON_COLOR);
}

Shop::~Shop() {
	delete _shop_item;
	_shop_item = nullptr;

	delete _sell_button;
	_sell_button = nullptr;

	delete _buy_button;
	_buy_button = nullptr;
}

void Shop::open() {
	update_text();

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

void Shop::input() {
	InputManager *input_manager = Environment::get().get_input_manager();

	_exit = !input_manager->get();

	if (input_manager->is_mouse(SDL_BUTTON_LEFT)) {
		SDL_Rect mouse_rect = { input_manager->get_mouse_x(), input_manager->get_mouse_y(), 1, 1 }; 

		if (collision(mouse_rect, _sell_button->get_rect())) {
			sell_item();
		}
		else if (collision(mouse_rect, _buy_button->get_rect())) {
			buy_item();
		}
		else if (!select_item_player(mouse_rect.x, mouse_rect.y)) {
			select_item_shop(mouse_rect.x, mouse_rect.y);
		}
	}

	if (input_manager->is_key(SDL_SCANCODE_E)) {
		if (_selection.player) {
			sell_item();
		}
		else {
			buy_item();
		}
	}
}

void Shop::update() {}

void Shop::render() {
	Renderer *renderer = Environment::get().get_window()->get_renderer();

	renderer->clear();

	Environment::get().get_resource_manager()->render();

	renderer->draw_rect(_background, BACKGROUND_COLOR, DRAW_RECT_FULL);
	renderer->draw_rect(_items_background, BACKGROUND_COLOR, DRAW_RECT_FULL);
	renderer->draw_rect(_shop_items_background, BACKGROUND_COLOR, DRAW_RECT_FULL);

	render_items();
	if (!_selection_info.hide) {
		render_item_info();
	}

	render_entities();

	render_buttons();

	renderer->render();
}

void Shop::render_entities() {
	Renderer *renderer = Environment::get().get_window()->get_renderer();

	SpriteComponent *sprite = GetSprite(_player);

	SDL_Rect center_rect = { (_background.w / 2) + ENTITY_LEFT_XOFFSET, (_background.h / 2) + ENTITY_LEFT_YOFFSET, ENTITY_LEFT_WIDTH, ENTITY_LEFT_HEIGHT };

	if (sprite) {
		renderer->render(Environment::get().get_resource_manager()->get_sprite_info(_player), sprite, center_rect, true);
	}
	else {
		renderer->render(Environment::get().get_resource_manager()->get_texture_info(_player), center_rect);
	}

	sprite = GetSprite(_shop_owner);
	PositionComponent *position = GetPosition(_shop_owner);
	center_rect = { (_background.w / 2) + ENTITY_RIGHT_XOFFSET, (_background.h / 2) + ENTITY_RIGHT_YOFFSET, ENTITY_RIGHT_WIDTH, ENTITY_RIGHT_HEIGHT * (position->rect.h / 32)  };

	if (sprite) {
		renderer->render(Environment::get().get_resource_manager()->get_sprite_info(_shop_owner), sprite, center_rect, true);
	}
	else {
		renderer->render(Environment::get().get_resource_manager()->get_texture_info(_shop_owner), center_rect);
	}
}

void Shop::render_items() {
	Renderer *renderer = Environment::get().get_window()->get_renderer();

	int x = _items_background.x;
	int y = _items_background.y;
	for (unsigned int i = 0; i < _inventory->size(); ++i) {
		if (i % MAX_ROW_SIZE == 0 && i != 0) {
			y += ITEM_SIZE;
			x = _items_background.x;
		}
		else if (i != 0) {
			x += ITEM_SIZE;
		}
		SDL_Rect item_rect = { x, y, ITEM_SIZE, ITEM_SIZE };
		renderer->render(Environment::get().get_resource_manager()->get_texture_info(_inventory->at(i)), item_rect, true);
	}

	x = _shop_items_background.x;
	y = _shop_items_background.y;
	for (unsigned int i = 0; i < _shop->size(); ++i) {
		if (i % MAX_ROW_SIZE == 0 && i != 0) {
			y += ITEM_SIZE;
			x = _shop_items_background.x;
		}
		else if (i != 0) {
			x += ITEM_SIZE;
		}
		SDL_Rect item_rect = { x, y, ITEM_SIZE, ITEM_SIZE };
		renderer->render(Environment::get().get_resource_manager()->get_texture_info(TYPE_ITEM, _shop->at(i)), item_rect, true);
	}

	if (_selection.index != -1) {
		SDL_Rect selection_rect;
		if (!_selection.hide) {
			selection_rect = { _background.w / 2 + SELECTION_XOFFSET, _background.h / 2 + SELECTION_YOFFSET, SELECTION_SIZE, SELECTION_SIZE };
			renderer->draw_rect(selection_rect, SELECTION_MAIN_COLOR);

			if (_selection.player) {
				renderer->render(Environment::get().get_resource_manager()->get_texture_info(_inventory->at(_selection.index)), selection_rect, true);
			}
			else {
				renderer->render(Environment::get().get_resource_manager()->get_texture_info(TYPE_ITEM, _shop->at(_selection.index)), selection_rect, true);
			}
		}

		int selection_x = _selection.index % MAX_ROW_SIZE;
		int selection_y = _selection.index / MAX_ROW_SIZE;
		if (_selection.player) {
			selection_rect = { selection_x * ITEM_SIZE + _items_background.x, selection_y * ITEM_SIZE + _items_background.y, ITEM_SIZE, ITEM_SIZE };
		}
		else {
			selection_rect = { selection_x * ITEM_SIZE + _shop_items_background.x, selection_y * ITEM_SIZE + _shop_items_background.y, ITEM_SIZE, ITEM_SIZE };
		}

		if (_selection.index != -1) {
			renderer->draw_rect(selection_rect, SELECTION_COLOR);
		}
	}

	renderer->draw_text(&_inventory_size, true);
	renderer->draw_text(&_gold, true);
}

void Shop::render_item_info() {
	if (_selection.index == -1) {
		return;
	}

	Renderer *renderer = Environment::get().get_window()->get_renderer();

	ItemComponent *item = nullptr;
	if (_selection.player)
		item = GetItem(_inventory->at(_selection.index));
	else {
		item = GetItem(_shop_item);
	}

	renderer->draw_text(&_selection_info.name, true);
	if (item->info.size() > 0)	renderer->draw_text(&_selection_info.info, true);
	if (item->health > 0)	  { renderer->draw_text(&_selection_info.health, true);			renderer->draw_text(&_selection_info.health_val, true);     }
	if (item->mana > 0)		  { renderer->draw_text(&_selection_info.mana, true);			renderer->draw_text(&_selection_info.mana_val, true);       }
	if (item->damage > 0)	  { renderer->draw_text(&_selection_info.damage, true);			renderer->draw_text(&_selection_info.damage_val, true);     }
	if (item->armor > 0)	  { renderer->draw_text(&_selection_info.armor, true);			renderer->draw_text(&_selection_info.armor_val, true);      }
	if (item->hps > 0)		  { renderer->draw_text(&_selection_info.hps, true);			renderer->draw_text(&_selection_info.hps_val, true);        }
	if (item->mps > 0)		  { renderer->draw_text(&_selection_info.mps, true);			renderer->draw_text(&_selection_info.mps_val, true);         }
	if (item->drain > 0)	  { renderer->draw_text(&_selection_info.drain, true);			renderer->draw_text(&_selection_info.drain_val, true);      }
	if (item->speed > 0)	  { renderer->draw_text(&_selection_info.speed, true);			renderer->draw_text(&_selection_info.speed_val, true);	    }
	if (item->luck > 0)		  { renderer->draw_text(&_selection_info.luck, true);			renderer->draw_text(&_selection_info.luck_val, true);       }
	if (item->duration > 0)   { renderer->draw_text(&_selection_info.duration, true);		renderer->draw_text(&_selection_info.duration_val, true);   }
	if (_selection.player)	    renderer->draw_text(&_selection_info.sell_value_val, true);
	else if (!_selection.player)renderer->draw_text(&_selection_info.buy_value_val, true);
	if (item->is_stackable)		renderer->draw_text(&_selection_info.stack_size_val, true);
}

void Shop::render_buttons() {
	Renderer *renderer = Environment::get().get_window()->get_renderer();

	renderer->draw_rect(_sell_button->get_rect(), _sell_button->get_color());
	renderer->draw_text(&_sell_button->get_text(), true);
	renderer->draw_rect(_buy_button->get_rect(), _buy_button->get_color());
	renderer->draw_text(&_buy_button->get_text(), true);
}

bool Shop::select_item_player(int mouse_x, int mouse_y) {
	SDL_Rect mouse_rect = { mouse_x, mouse_y, 1, 1 };

	if (mouse_x < _items_background.x || mouse_x > _items_background.x + _items_background.w ||
		mouse_y < _items_background.y || mouse_y > _items_background.y + _items_background.h) {
		_selection.index = -1;
		_selection.hide = true;
		return false;
	}

	int x = (mouse_x - _items_background.x) / ITEM_SIZE;
	int y = (mouse_y - _items_background.y) / ITEM_SIZE;
	_selection.index = x + y * MAX_ROW_SIZE;

	if (_selection.index >= (int)_inventory->size()) {
		_selection.index = -1;
		_selection.hide = true;
		return false;
	}

	_selection.player = true;
	_selection.hide = false;

	set_item_info(GetItem(_inventory->at(_selection.index)));
	return true;
}

bool Shop::select_item_shop(int mouse_x, int mouse_y) {
	SDL_Rect mouse_rect = { mouse_x, mouse_y, 1, 1 };

	if (mouse_x < _shop_items_background.x || mouse_x > _shop_items_background.x + _shop_items_background.w ||
		mouse_y < _shop_items_background.y || mouse_y > _shop_items_background.y + _shop_items_background.h) {
		_selection.index = -1;
		_selection.hide = true;
		return false;
	}

	int x = (mouse_x - _shop_items_background.x) / ITEM_SIZE;
	int y = (mouse_y - _shop_items_background.y) / ITEM_SIZE;
	_selection.index = x + y * MAX_ROW_SIZE;

	if (_selection.index >= (int)_shop->size()) {
		_selection.index = -1;
		_selection.hide = true;
		return false;
	}

	_selection.player = false;
	_selection.hide = false;

	if (_shop_item) {
		delete _shop_item;
		_shop_item = nullptr;
	}
	_shop_item = new Entity(TYPE_ITEM, _shop->at(_selection.index));
	set_item_info(GetItem(_shop_item));
	return true;
}

void Shop::set_item_info(ItemComponent *item) {
	if (!item) {
		_selection_info.hide = true;
		return;
	}

	int font_size = ITEM_INFO_FT_SIZE;
	int wrap_length = ITEM_INFO_WRAP_LENGTH;
	int label_x = (_background.w / 2 + SELECTION_XOFFSET + SELECTION_SIZE / 2) + ITEM_INFO_LABEL_XOFFSET;
	int val_x = (_background.w / 2 + SELECTION_XOFFSET + SELECTION_SIZE / 2) + ITEM_INFO_VAL_XOFFSET;
	int y = _background.h / 2 + SELECTION_YOFFSET + ITEM_INFO_YOFFSET;

	_selection_info.name = Text(item->name, item->color, font_size, wrap_length, _background.w / 2 + SELECTION_XOFFSET + SELECTION_SIZE / 2, y);
	y += (font_size + 10) * item->name.size() / 15;

	_selection_info.info = Text(item->info, item->color, font_size, wrap_length, _background.w / 2 + SELECTION_XOFFSET + SELECTION_SIZE / 2, y);
	y += font_size + 2 + (font_size + 2) * (item->info.size() / 20);


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

	if (item->duration > 0) {
		_selection_info.duration = Text("Duration:", DURATION_COLOR, font_size, wrap_length, label_x, y);
		_selection_info.duration_val = Text(std::to_string(item->duration / 1000) + "s", DURATION_COLOR, font_size, wrap_length, val_x, y);
		y += font_size + 2;
	}

	if (_selection.player) {
		_selection_info.sell_value_val = Text(std::to_string(item->sell_value) + "g", VALUE_COLOR, font_size, wrap_length, val_x, y);
		y += font_size + 2;
	}
	else {
		_selection_info.buy_value_val = Text(std::to_string(item->buy_value) + "g", VALUE_COLOR, font_size, wrap_length, val_x, y);
		y += font_size + 2;
	}

	if (item->is_stackable) {
		_selection_info.stack_size_val = Text("x" + std::to_string(item->stack_size), STACK_SIZE_COLOR, font_size, wrap_length, val_x, y);
		y += font_size + 2;
	}

	_selection_info.hide = false;
}

void Shop::sell_item() {
	if (_selection.index == -1 || !_selection.player) {
		return;
	}

	ItemComponent *item = GetItem(_inventory->at(_selection.index));
	PlayerComponent *player = GetPlayer(Environment::get().get_resource_manager()->get_player());

	player->gold += item->sell_value;

	if (item->stack_size > 1) {
		--item->stack_size;
		return;
	}

	delete _inventory->at(_selection.index);
	_inventory->erase(_inventory->begin() + _selection.index);
	_selection.index = -1;

	update_text();
}

void Shop::buy_item() {
	if (_selection.index == -1 || _selection.player || !_shop_item) {
		return;
	}

	ItemComponent *item = GetItem(_shop_item);
	PlayerComponent *player = GetPlayer(Environment::get().get_resource_manager()->get_player());

	if (player->gold >= item->buy_value && player->items.size() < MAX_ITEMS) {
		player->gold -= item->buy_value;
		player->inventory.add_item(_shop_item->get_type_id());
	}

	update_text();
}

void Shop::update_text() {
	_inventory_size = Text(std::to_string(_inventory->size()) + "/" + std::to_string(MAX_ITEMS),
		INV_SIZE_COLOR, INV_SIZE_FT_SIZE, INV_SIZE_WRAP_LENGTH, _items_background.x + _items_background.w, _items_background.y + _items_background.h + INV_SIZE_FT_SIZE + 2);

	PlayerComponent *player = GetPlayer(Environment::get().get_resource_manager()->get_player());
	_gold = Text(std::to_string(player->gold) + "g", VALUE_COLOR, ITEM_INFO_FT_SIZE, ITEM_INFO_WRAP_LENGTH, _items_background.x + _items_background.w - GOLD_XOFFSET, _items_background.y - ITEM_INFO_FT_SIZE);
}
