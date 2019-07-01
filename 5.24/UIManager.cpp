#include "UIManager.h"

#include "Environment.h"
#include "ResourceManager.h"
#include "Window.h"
#include "Log.h"

#include "Collision.h"

#include "UIHandler.h"
#include "Button_Pressable.h"

#define MOUSE_WIDTH 1
#define MOUSE_HEIGHT 1

#define BUTTON_CONFIRM "Yes"
#define BUTTON_CONFIRM_XOFFSET 260
#define BUTTON_CONFIRM_YOFFSET 60
#define BUTTON_CONFIRM_W 120
#define BUTTON_CONFIRM_H 30
#define BUTTON_CONFIRM_FT_SIZE 24
#define BUTTON_DENY "No"

#define CURRENT_TEXT_COLOR {255, 255, 255, 255}
#define CURRENT_TEXT_FT_SIZE 24
#define CURRENT_TEXT_YOFFSET 200

#define ELEMENT_SELECTION_HEIGHT 256
#define ELEMENT_AREA_COLOR { 100, 100, 100, 100 }

UIManager::UIManager() :
	_state					( STATE_IDLE ),
	_element_area			( { {Environment::get().get_window()->get_width() - ELEMENT_AREA_WIDTH, 0, ELEMENT_AREA_WIDTH, Environment::get().get_window()->get_height()},
								 ELEMENT_AREA_COLOR,
								{Environment::get().get_window()->get_width() - ELEMENT_AREA_WIDTH, ELEMENT_SELECTION_HEIGHT, ELEMENT_AREA_WIDTH, Environment::get().get_window()->get_height()},
								{Environment::get().get_window()->get_width() - ELEMENT_AREA_WIDTH, 0, ELEMENT_AREA_WIDTH, ELEMENT_SELECTION_HEIGHT} } ),
	_placement				( { TYPE_TILE, -1 } ),
	_center_placement		( false )
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

bool UIManager::check_buttons(const int &mouse_x, const int &mouse_y) {
	for (auto it = _buttons.begin(); it != _buttons.end(); it++) {
		if (collision(it->second->get_rect(), { mouse_x, mouse_y, MOUSE_WIDTH, MOUSE_HEIGHT })) {
			it->second->execute();
			return true;
		}
	}

	return false;
}

bool UIManager::check_selection(const int &mouse_x, const int &mouse_y) {
	if (mouse_x >= _element_area.area.x) {
		_placement.id = select_placement(mouse_x, mouse_y);
		if (_placement.id != -1) {
			_state = STATE_PLACING;
		}
		else {
			_state = STATE_IDLE;
		}
		return true;
	}

	if (_state == STATE_PLACING) {
		place_element(mouse_x, mouse_y);
		return true;
	}

	if (_state = STATE_SELECTING) {
		_selection = select_element(mouse_x, mouse_y);
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
	if(_state == STATE_WAITING)
		renderer->draw_text(_current_text, true);

	Environment::get().get_resource_manager()->render_editor(_element_area, _placement);
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

int UIManager::select_placement(const int &mouse_x, const int &mouse_y) {
	if (_state == STATE_WAITING) {
		return -1;
	}
	if (mouse_y < _element_area.area.y) {
		return -1;
	}
	int index = int((mouse_x - _element_area.area.x) / ELEMENT_WIDTH) + int(((mouse_y - _element_area.area.y) / ELEMENT_HEIGHT) * ELEMENT_ROW_SIZE);

	if (index < 0 || index > (int)Environment::get().get_resource_manager()->get_surface_size(_placement.type) - 1) {
		return -1;
	}

	return index;
}

bool UIManager::place_element(const int &mouse_x, const int &mouse_y) {
	if (_state == STATE_WAITING) {
		return false;
	}
	SDL_Rect boundry = Environment::get().get_resource_manager()->get_map()->get_rect();
	float scale = Environment::get().get_window()->get_camera()->get_scale();
	double x = (mouse_x / scale) + Environment::get().get_window()->get_camera()->get_x();
	double y = (mouse_y / scale) + Environment::get().get_window()->get_camera()->get_y();

	if (x < 0 || x > boundry.x + boundry.w || y < 0 || y > boundry.y + boundry.h) {
		return false;
	}

	int index = int(x / TILE_WIDTH) + ((int(y / TILE_HEIGHT) * int(boundry.w / TILE_WIDTH)));

	if (_placement.type == TYPE_TILE) {
		Environment::get().get_resource_manager()->edit_map(index, _placement.id);
		return true;
	}

	if (_center_placement) {
		int map_width = Environment::get().get_resource_manager()->get_map()->get_width();
		x = (index % map_width) * TILE_WIDTH + (TILE_WIDTH / 2);
		y = (index / map_width) * TILE_HEIGHT + (TILE_HEIGHT / 2);
	}

	if (_placement.type == TYPE_OBJECT) {
		Environment::get().get_resource_manager()->create(_placement.type, _placement.id, x, y);
	}

	return true;
}

Element UIManager::select_element(const int &mouse_x, const int &mouse_y) {
	float scale = Environment::get().get_window()->get_camera()->get_scale();
	double x = (mouse_x / scale) + Environment::get().get_window()->get_camera()->get_x();
	double y = (mouse_y / scale) + Environment::get().get_window()->get_camera()->get_y();
	std::map<int, Entity *> *entities = Environment::get().get_resource_manager()->get_entities();
	SDL_Rect mouse = { (int)x, (int)y, MOUSE_WIDTH, MOUSE_HEIGHT };
	for (auto it = entities->begin(); it != entities->end(); it++) {
		if (PositionComponent *position = GetPosition(it->second)) {
			if (collision(mouse, position->rect)) {
				return { it->second->get_type(), it->second->get_id() };
			}
		}

	}

	return { " " , -1 };
}

void UIManager::delete_element() {
	if (_selection.id != -1) {
		Environment::get().get_resource_manager()->remove(_selection.type, _selection.id);
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