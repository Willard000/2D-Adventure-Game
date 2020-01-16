#include "Inventory.h"

#include "Button.h"
#include "Button_Pressable.h"

#include  <vector>

#ifndef SHOP_H
#define SHOP_H

class Shop {
public:
	struct Selection {
		int index = -1;
		bool player = false;
		bool hide = true;
	};

	Shop(Entity *player, std::vector<Entity *> *inventory, Entity *shop_owner, std::vector<int> *shop);
	~Shop();

	void open();

private:
	void input();
	void update();
	void render();

	void render_entities();
	void render_items();
	void render_item_info();
	void render_buttons();

	bool select_item_player(int mouse_x, int mouse_y);
	bool select_item_shop(int mouse_x, int mouse_y);
	void set_item_info(ItemComponent *item);

	void update_text();

	void sell_item();
	void buy_item();

	bool _exit;

	Entity* _player;
	Entity* _shop_owner;
	std::vector<Entity *> *_inventory;
	std::vector<int> *_shop;

	Entity * _shop_item;

	SDL_Rect _background;
	SDL_Rect _items_background;
	SDL_Rect _shop_items_background;

	Selection _selection;
	Inventory::Item_Info _selection_info;

	UI::Button *_sell_button;
	UI::Button *_buy_button;

	Text _inventory_size;
	Text _gold;
};

#endif