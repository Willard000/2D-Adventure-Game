#include "UIManager.h"

#include "Environment.h"
#include "ResourceManager.h"
#include "Window.h"
#include "Log.h"
#include "Clock.h"
#include "InputManager.h"

#include "Collision.h"

#include "UIHandler.h"
#include "Button_Pressable.h"

#include "Timer.h"

#define MOUSE_WIDTH 1
#define MOUSE_HEIGHT 1

#define BUTTON_CONFIRM "Yes"
#define BUTTON_CONFIRM_XOFFSET 260
#define BUTTON_CONFIRM_YOFFSET 60
#define BUTTON_CONFIRM_W 120
#define BUTTON_CONFIRM_H 30
#define BUTTON_CONFIRM_FT_SIZE 24
#define BUTTON_DENY "No"

#define CURRENT_TEXT_COLOR { 255, 255, 255, 255 }
#define CURRENT_TEXT_FT_SIZE 24
#define CURRENT_TEXT_YOFFSET 200

#define MOUSE_TEXT_TIME 100
#define MOUSE_TEXT_COLOR { 255, 100, 255, 255 }
#define MOUSE_TEXT_XOFFSET 50
#define MOUSE_TEXT_YOFFSET -20
#define MOUSE_TEXT_PTSIZE 14

#define ELEMENT_SELECTION_COLOR { 0, 250, 240, 50 }
#define ELEMENT_SELECTION_HEIGHT 256
#define ELEMENT_AREA_COLOR { 100, 100, 100, 100 }

#define WARP_COLOR {0, 255, 0, 100}

SDL_Rect get_mouse_rect(const int &x, const int &y) {
	return { x, y, MOUSE_WIDTH, MOUSE_HEIGHT };
}

bool element_collision(const int &x, const int &y) {
	SDL_Rect placement = get_mouse_rect(x, y);

	std::map<std::string, EntityManager::Entity_Map> *entities = Environment::get().get_resource_manager()->get_entities();
	for (auto it = entities->begin(); it != entities->end(); it++) {
		for (auto itt = it->second.begin(); itt != it->second.end(); itt++) {
			if (PositionComponent *position = GetPosition(itt->second)) {
				if (collision(placement, position->rect)) {
					return true;
				}
			}
		}
	}

	return false;
}

UIManager::UIManager() :
	_state					( STATE_IDLE ),
	_element_area			( { {Environment::get().get_window()->get_width() - ELEMENT_AREA_WIDTH, 0, ELEMENT_AREA_WIDTH, Environment::get().get_window()->get_height()},
								 ELEMENT_AREA_COLOR,
								{Environment::get().get_window()->get_width() - ELEMENT_AREA_WIDTH, ELEMENT_SELECTION_HEIGHT, ELEMENT_AREA_WIDTH, Environment::get().get_window()->get_height()},
								{Environment::get().get_window()->get_width() - ELEMENT_AREA_WIDTH, 0, ELEMENT_AREA_WIDTH, ELEMENT_SELECTION_HEIGHT} } ),
	_placement				( { TYPE_TILE, -1 } ),
	_selection				( { TYPE_OBJECT, -1 } ),
	_center_placement		( true ),
	_mouse_x				( 0 ),
	_mouse_y				( 0 ),
	_on_confirm				( nullptr ),
	_on_deny				( nullptr )
{
	Environment::get().get_log()->print("Loading UI Manager");
}

UIManager::~UIManager() {
	Environment::get().get_log()->print("Closing UI Manager");

	for (auto it = _buttons.begin(); it != _buttons.end(); it++) {
		delete it->second;
	}

}

void UIManager::add_button(Button *button) {
	_buttons[button->get_text().text] = button;
}

void UIManager::remove_button(std::string key) {
	if (_buttons[key]) {
		delete _buttons[key];
		_buttons.erase(key);
		return;
	}

	Environment::get().get_log()->print("Couldn't remove button at key - " + key);
}

int UIManager::update() {
	SDL_GetMouseState(&_mouse_x, &_mouse_y);
	update_mouse_location();

	if (_state == STATE_CONFIRM || _state == STATE_DENY) {
		pop_confirmation();

		if (_state == STATE_CONFIRM && _on_confirm) {
			_state = STATE_WAITING;
			_on_confirm();
			_on_confirm = nullptr;
		}
		else if (_state == STATE_DENY && _on_deny) {
			_state = STATE_WAITING;
			_on_deny();
			_on_deny = nullptr;
		}

		_state = _state_prev;
	}

	return _state;
}

void UIManager::update_mouse_location() {
	static Timer mouse_update(MOUSE_TEXT_TIME);

	if (mouse_update.update()) {
		int x, y;
		calc_real_mouse_location(x, y);
		x = x / TILE_WIDTH + 1, y = y / TILE_HEIGHT + 1;

		_mouse_location = Text(
			"x: " + std::to_string(x) + " y: " + std::to_string(y),
			MOUSE_TEXT_COLOR,
			MOUSE_TEXT_PTSIZE,
			1000,
			MOUSE_TEXT_XOFFSET,
			Environment::get().get_window()->get_height() + MOUSE_TEXT_YOFFSET
		);
	}
}

bool UIManager::check_buttons() {
	for (auto it = _buttons.begin(); it != _buttons.end(); it++) {
		if (collision(it->second->get_rect(), get_mouse_rect(_mouse_x, _mouse_y))) {
			it->second->execute();
			return true;
		}
	}

	return false;
}

bool UIManager::check_selection() {
	if (_mouse_x >= _element_area.area.x) {
		_placement.id = select_placement();
		if (_placement.id != -1) {
			_state = STATE_PLACING;
		}
		else {
			_state = STATE_IDLE;
		}
		return true;
	}

	if (_state == STATE_PLACING) {
		place_element();
		return true;
	}

	if (_state = STATE_SELECTING) {
		_selection = select_element();
		return true;
	}

	return false;
}

void UIManager::render() {
	Renderer *renderer = Environment::get().get_window()->get_renderer();

	for (auto it = _buttons.begin(); it !=_buttons.end(); it++) {
		renderer->draw_rect(it->second->get_rect(), it->second->get_color());
		renderer->draw_text(it->second->get_text(), true);
	}


	renderer->draw_text(_mouse_location, true);

	Environment::get().get_resource_manager()->render_editor(_element_area, _placement);
	if (_state == STATE_WAITING)
		renderer->draw_text(_current_text, true);
	else if (_state == STATE_PLACING) {
		int x, y;
		calc_real_mouse_location(x, y);
		Texture *img = Environment::get().get_resource_manager()->get_texture_info(_placement.type, _placement.id);
		if (img != nullptr) {
			renderer->render(img, { (int)x, (int)y, TILE_WIDTH, TILE_HEIGHT });
		}
	}
	else if (_state == STATE_SELECTING) {
		if (_selection.id != -1) {
			Entity *entity = Environment::get().get_resource_manager()->get_entity(_selection.type, _selection.id);
			if (PositionComponent *position = GetPosition(entity)) {
				renderer->draw_rect(position->rect, ELEMENT_SELECTION_COLOR, DRAW_RECT_CAMERA);
			}
		}
	}
}

void UIManager::set_state(int flag) {
	_state = flag;
}

void UIManager::set_current_text(std::string text) {
	int width_half = Environment::get().get_window()->get_width_half();
	int height_half = Environment::get().get_window()->get_height_half();
	_current_text = Text (text, CURRENT_TEXT_COLOR, CURRENT_TEXT_FT_SIZE, 0, width_half, height_half - CURRENT_TEXT_YOFFSET);
}

void UIManager::push_confirmation(Callback on_confirm, Callback on_deny) {
	_on_confirm = on_confirm;
	_on_deny = on_deny;
	int width = Environment::get().get_window()->get_width();
	int height = Environment::get().get_window()->get_height();
	Button_Pressable *button_confirm = new Button_Pressable(
		&confirm_yes,
		BUTTON_CONFIRM,
		{ (width / 2) - BUTTON_CONFIRM_XOFFSET, (height / 2) - BUTTON_CONFIRM_YOFFSET, BUTTON_CONFIRM_W, BUTTON_CONFIRM_H },
		BUTTON_CONFIRM_FT_SIZE
	);

	Button_Pressable *button_deny = new Button_Pressable(
		&confirm_no,
		BUTTON_DENY,
		{ (width / 2) + BUTTON_CONFIRM_XOFFSET - BUTTON_CONFIRM_W, (height / 2) - BUTTON_CONFIRM_YOFFSET, BUTTON_CONFIRM_W, BUTTON_CONFIRM_H },
		BUTTON_CONFIRM_FT_SIZE
	);

	add_button(button_confirm);
	add_button(button_deny);

	_state_prev = _state;
	_state = STATE_WAITING;
}

void UIManager::pop_confirmation() {
	remove_button(BUTTON_CONFIRM);
	remove_button(BUTTON_DENY);
}

int UIManager::select_placement() {
	if (_state == STATE_WAITING) {
		return -1;
	}
	if (_mouse_y < _element_area.area.y) {
		return -1;
	}
	int index = int((_mouse_x - _element_area.area.x) / ELEMENT_WIDTH) + int(((_mouse_y - _element_area.area.y) / ELEMENT_HEIGHT) * ELEMENT_ROW_SIZE);

	if (index < 0 || index > (int)Environment::get().get_resource_manager()->get_surface_size(_placement.type) - 1) {
		return -1;
	}

	return index;
}

bool UIManager::place_element() {
	if (_state == STATE_WAITING) {
		return false;
	}
	SDL_Rect boundry = Environment::get().get_resource_manager()->get_map()->get_rect();
	int x, y;
	calc_real_mouse_location(x, y);

	if (x < 0 || x > boundry.x + boundry.w || y < 0 || y > boundry.y + boundry.h) {
		return false;
	}

	int index = int(x / TILE_WIDTH) + ((int(y / TILE_HEIGHT) * int(boundry.w / TILE_WIDTH)));

	if (_placement.type == TYPE_TILE) {
		Environment::get().get_resource_manager()->edit_map(index, _placement.id);
		return true;
	}

	if (_placement.type == TYPE_SOLID) {
		Environment::get().get_resource_manager()->edit_solid(index);
		return true;
	}

	if (_placement.type == TYPE_WARP) {
		place_warp();
		return true;
	}

	if (_center_placement) {
		int map_width = Environment::get().get_resource_manager()->get_map()->get_width();
		x = (index % map_width) * TILE_WIDTH + (TILE_WIDTH / 2);
		y = (index / map_width) * TILE_HEIGHT + (TILE_HEIGHT / 2);
	}

	if (element_collision((int)x, (int)y)) {
		return false;
	}

	if (_placement.type == TYPE_OBJECT) {
		Environment::get().get_resource_manager()->create(_placement.type, _placement.id, x, y);
	}

	return true;
}

void UIManager::place_warp() {
	Map::Warp warp;
	Map *map = Environment::get().get_resource_manager()->get_map();
	int map_id = map->get_id();

	warp.from_id = map_id;
	warp.from = place_warp_rect();

	set_current_text("Enter Warp Map ID");
	Environment::get().get_input_manager()->get_text_input(&warp.to_id);

	map->save();

	if (!Environment::get().get_resource_manager()->load_map(warp.to_id)) {
		return;
	}

	warp.to = place_warp_rect();

	Environment::get().get_resource_manager()->load_map(map_id);

	map->add_warp(warp);

	_state = STATE_IDLE;
}

SDL_Rect UIManager::place_warp_rect() {
	_state = STATE_WAITING;
	set_current_text("Select Warp Start Point");

	bool end_input = false;
	bool start_point = true;
	bool end_point = false;

	SDL_Rect pos = { 0, 0, 0, 0 };

	int x, y;
	while (!end_point && (Environment::get().get_input_manager()->get())) {
		Environment::get().get_window()->get_renderer()->clear();

		Environment::get().get_ui_manager()->update();

		Environment::get().get_input_manager()->update_editor_camera();

		if ((Environment::get().get_input_manager()->is_mouse(SDL_BUTTON_LEFT))) {
			if (start_point) {
				calc_real_mouse_location(pos.x, pos.y);
				start_point = false;
				set_current_text("Select Warp End Point");
			}
			else {
				end_point = true;
			}
		}

		Environment::get().get_resource_manager()->render();
		Environment::get().get_ui_manager()->render();

		if (!start_point) {
			calc_real_mouse_location(x, y);
			pos.w = x - pos.x;
			pos.h = y - pos.y;

			Environment::get().get_window()->get_renderer()->draw_rect(pos, WARP_COLOR, DRAW_RECT_CAMERA);
		}

		Environment::get().get_window()->get_renderer()->render();

		if (Environment::get().get_clock()->update()) {
			std::string title = "Warp      Map: " + std::to_string(Environment::get().get_resource_manager()->get_map()->get_id()) + "     " +
				Environment::get().get_clock()->get_display_time() + "    " + std::to_string(Environment::get().get_clock()->get_fms());
			Environment::get().get_window()->set_title(title);
		}
	}
	 
	return pos;
}

Element UIManager::select_element() {
	int x, y;
	calc_real_mouse_location(x, y);

	std::map<std::string, EntityManager::Entity_Map> *entities = Environment::get().get_resource_manager()->get_entities();
	SDL_Rect mouse = get_mouse_rect((int)x, (int)y);
	for (auto it = entities->begin(); it != entities->end(); it++) {
		for (auto itt = it->second.begin(); itt != it->second.end(); itt++) {
			if (PositionComponent *position = GetPosition(itt->second)) {
				if (collision(mouse, position->rect)) {
					return { itt->second->get_type(), itt->second->get_id() };
				}
			}
		}
	}

	return { " " , -1 };
}

void UIManager::delete_element() {
	if (_selection.id != -1) {
		Environment::get().get_resource_manager()->remove(_selection.type, _selection.id);
		_selection.id = -1;
		_selection.type = " ";
	}
}

void UIManager::set_placement_type(std::string type) {
	_placement.type = type;
}

std::string UIManager::get_placement_type() {
	return _placement.type;
}

void UIManager::toggle_center_placement() {
	_center_placement = !_center_placement;
}

void UIManager::calc_real_mouse_location(int &x, int &y) {
	Camera *camera = Environment::get().get_window()->get_camera();
	x = int(double((_mouse_x / camera->get_scale()) + camera->get_x()));
	y = int(double((_mouse_y / camera->get_scale()) + camera->get_y()));
}