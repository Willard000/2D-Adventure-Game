#include <SDL.h>

#include <map>
#include <vector>

#include "Entity.h"
#include "Button.h"
#include "Path.h"

#include "Globals.h"


#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#define ELEMENT_WIDTH 32
#define ELEMENT_HEIGHT 32
#define ELEMENT_ROW_SIZE 8
#define ELEMENT_AREA_WIDTH ELEMENT_ROW_SIZE * ELEMENT_WIDTH

#define MOUSE_LEFT 0
#define MOUSE_RIGHT 1

#define MAP_SELECTION_ROTATION_FACTOR 10
#define MAP_SELECTION_SCALE_FACTOR .01f

namespace UI {

	struct Element_Area {
		SDL_Rect background;
		SDL_Color color;
		SDL_Rect area;
		SDL_Rect info;
	};

	struct Element {
		int type;
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

	void highlight_button();

	bool check_buttons();
	bool check_placement(int mouse_button);
	bool check_mass_placement(int mouse_button, int start_x, int start_y);
	void render();
	void render_current_text();

	void set_state(int flag);
	void set_current_text(std::string text);

	void push_confirmation(Callback on_confirm = nullptr, Callback on_deny = nullptr);
	void pop_confirmation();

	Element select_from_map();
	bool place_on_map();
	void delete_map_selection();

	bool mass_place_on_map(int start_x, int start_y);

	// returns element id ( -1 if mouse is not hovering an element)
	int select();

	void set_selection_type(int type);
	int get_selection_type();

	void toggle_alignment();
	void toggle_stacking();

	void place_warp();
	void place_enemy(float x = 0.0f, float y = 0.0f);
	void place_npc(float x = 0.0f, float y = 0.0f);

	std::vector<Path> get_path(Entity *entity);

	int get_state() { return _state; }

	void rotate_map_selection(float rotation);
	void scale_map_selection(float scale);
	void set_map_selection_rotation(float rotation);
	void set_map_selection_scale(float scale);

	void move_map_selection();

	SDL_Rect get_element_area() { return _element_area.background; }

	void unselect();
private:
	void get_real_mouse_location(int &x, int &y);
	void calc_real_mouse_location(int &x, int &y);
	void calc_align_mouse_location(int &x, int &y);
	SDL_Rect place_warp_rect();

	void update_map_selection_text();

	void set_name_text();
private:
	int _state;
	int _state_prev;

	int _mouse_button;
	int _mouse_x;
	int _mouse_y;

	Text _current_text;
	Text _mouse_location;
	Text _alignment_text;
	Text _selection_text;
	Text _stacking_text;
	Text _rotation_text;
	Text _scale_text;
	Text _name_text;

	Callback _on_confirm;
	Callback _on_deny;

	Element_Area _element_area;
	Element _selection;		
	Element _map_selection;			

	Button *_highlighted_button;

	bool _align_placement;
	bool _allow_stacking;

	std::map<std::string, Button *> _buttons;

	// remembers last selection for each type
	// Key - placement.type Val - placement.id
	std::map<int, int> _last_selected_id;
};


#endif