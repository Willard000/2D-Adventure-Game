#include <SDL.h>

#include <map>

#include "Button.h"

#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#define ELEMENT_WIDTH 32
#define ELEMENT_HEIGHT 32
#define ELEMENT_ROW_SIZE 8
#define ELEMENT_AREA_WIDTH ELEMENT_ROW_SIZE * ELEMENT_WIDTH

namespace UI {

	struct Element_Area {
		SDL_Rect background;
		SDL_Color color;
		SDL_Rect area;
		SDL_Rect info;
	};

	struct Selection {
		bool is;
		std::string type;
		int id;
	};

	enum {
		STATE_IDLE,
		STATE_CONFIRM,
		STATE_DENY,
		STATE_WAITING,
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
	bool check_buttons(const int &mouse_x, const int &mouse_y);
	bool check_selection(const int &mouse_x, const int &mouse_y);
	void render();

	void set_state(int flag);
	void set_current_text(std::string text);

	void push_confirmation(Callback on_confirm = nullptr, Callback on_deny = nullptr);
	void pop_confirmation();

	// returns element id ( -1 if mouse is not hovering an element)
	int select_element(const int &mouse_x, const int &mouse_y);

	bool place_element(const int &mouse_x, const int &mouse_Y);

	int get_state() { return _state; }
private:
	int _state;

	Text _current_text;

	Callback _on_confirm;
	Callback _on_deny;

	Element_Area _element_area;
	Selection _selection;

	std::map<std::string, Button *> _buttons;
};


#endif