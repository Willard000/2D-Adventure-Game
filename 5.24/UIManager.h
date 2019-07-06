#include <SDL.h>

#include <map>

#include "Button.h"

#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#define ELEMENT_WIDTH 32
#define ELEMENT_HEIGHT 32
#define ELEMENT_ROW_SIZE 8
#define ELEMENT_AREA_WIDTH ELEMENT_ROW_SIZE * ELEMENT_WIDTH

#define TYPE_SOLID "Solid"
#define TYPE_WARP "Warp"

namespace UI {

	struct Element_Area {
		SDL_Rect background;
		SDL_Color color;
		SDL_Rect area;
		SDL_Rect info;
	};

	struct Element {
		std::string type;
		int id;
	};

	enum {
		STATE_IDLE,
		STATE_CONFIRM,
		STATE_DENY,
		STATE_WAITING,
		STATE_PLACING,
		STATE_SELECTING,
		STATE_PLACE_WARP
	};

}

using namespace UI;

class UIManager {
public:
	typedef void(*Callback)(void);

	UIManager();
	~UIManager();

	void add_button(Button *button);

	void remove_button(std::string key);

	int update();

	void update_mouse_location();

	bool check_buttons();
	bool check_selection();
	void render();

	void set_state(int flag);
	void set_current_text(std::string text);

	void push_confirmation(Callback on_confirm = nullptr, Callback on_deny = nullptr);
	void pop_confirmation();

	Element select_element();
	void delete_element();

	// returns element id ( -1 if mouse is not hovering an element)
	int select_placement();
	bool place_element();
	void set_placement_type(std::string type);
	std::string get_placement_type();

	void toggle_center_placement();

	void place_warp();


	int get_state() { return _state; }
private:
	void calc_real_mouse_location(int &x, int &y);
	SDL_Rect place_warp_rect();
private:
	int _state;
	int _state_prev;

	int _mouse_x;
	int _mouse_y;

	Text _current_text;
	Text _mouse_location;

	Callback _on_confirm;
	Callback _on_deny;

	Element_Area _element_area;
	Element _placement;
	Element _selection;

	bool _center_placement;

	std::map<std::string, Button *> _buttons;
};


#endif