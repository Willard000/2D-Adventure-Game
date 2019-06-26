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
	_element_area			( { {Environment::get().getWindow()->getWidth() - ELEMENT_AREA_WIDTH, 0, ELEMENT_AREA_WIDTH, Environment::get().getWindow()->getHeight()},
								 ELEMENT_AREA_COLOR,
								{Environment::get().getWindow()->getWidth() - ELEMENT_AREA_WIDTH, ELEMENT_SELECTION_HEIGHT, ELEMENT_AREA_WIDTH, Environment::get().getWindow()->getHeight()},
								{Environment::get().getWindow()->getWidth() - ELEMENT_AREA_WIDTH, 0, ELEMENT_AREA_WIDTH, ELEMENT_SELECTION_HEIGHT} } ),
	_selection				( { false, TYPE_TILE, -1 } )
{
	Environment::get().getLog()->print("Loading UI Manager");
}

UIManager::~UIManager() {
	Environment::get().getLog()->print("Closing UI Manager");

	for (auto it = _buttons.begin(); it != _buttons.end(); it++) {
		delete it->second;
	}

}

void UIManager::add_button(Button *button) {
	_buttons[button->getText().text] = button;
}

void UIManager::remove_button(std::string key) {
	if (_buttons[key]) {
		delete _buttons[key];
		_buttons.erase(key);
		return;
	}

	Environment::get().getLog()->print("Couldn't remove button at key - " + key);
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

		_state = STATE_IDLE;
	}

	return _state;
}

bool UIManager::check_buttons(const int &mouse_x, const int &mouse_y) {
	for (auto it = _buttons.begin(); it != _buttons.end(); it++) {
		if (collision(it->second->getRect(), { mouse_x, mouse_y, MOUSE_WIDTH, MOUSE_HEIGHT })) {
			it->second->execute();
			return true;
		}
	}

	return false;
}

bool UIManager::check_selection(const int &mouse_x, const int &mouse_y) {
	if (mouse_x >= _element_area.area.x) {
		_selection.id = select_element(mouse_x, mouse_y);
		if (_selection.id != -1) {
			_selection.is = true;
		}
		else {
			_selection.is = false;
		}
		return true;
	}

	if (_selection.is) {
		place_element(mouse_x, mouse_y);
		return true;
	}

	return false;
}

void UIManager::render() {
	Renderer *renderer = Environment::get().getWindow()->getRenderer();

	for (auto it = _buttons.begin(); it !=_buttons.end(); it++) {
		renderer->drawRect(it->second->getRect(), it->second->getColor());
		renderer->drawText(it->second->getText(), true);
	}
	if(_state == STATE_WAITING)
		renderer->drawText(_current_text, true);

	Environment::get().getResourceManager()->renderEditor(_element_area, _selection);
}

void UIManager::set_state(int flag) {
	_state = flag;
}

void UIManager::set_current_text(std::string text) {
	int width_half = Environment::get().getWindow()->getWidthHalf();
	int height_half = Environment::get().getWindow()->getHeightHalf();
	_current_text = Text (text, CURRENT_TEXT_COLOR, CURRENT_TEXT_FT_SIZE, 0, width_half, height_half - CURRENT_TEXT_YOFFSET);
}

void UIManager::push_confirmation(Callback on_confirm, Callback on_deny) {
	_on_confirm = on_confirm;
	_on_deny = on_deny;
	int width = Environment::get().getWindow()->getWidth();
	int height = Environment::get().getWindow()->getHeight();
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
	_state = STATE_WAITING;
}

void UIManager::pop_confirmation() {
	remove_button(BUTTON_CONFIRM);
	remove_button(BUTTON_DENY);
}

int UIManager::select_element(const int &mouse_x, const int &mouse_y) {
	if (mouse_y < _element_area.area.y) {
		return -1;
	}
	int index = int((mouse_x - _element_area.area.x) / ELEMENT_WIDTH) + int(((mouse_y - _element_area.area.y) / ELEMENT_HEIGHT) * ELEMENT_ROW_SIZE);
	if (index < 0 || index > (int)Environment::get().getResourceManager()->getSurfaceSize(_selection.type)) {
		return -1;
	}
	return index;
}

bool UIManager::place_element(const int &mouse_x, const int &mouse_y) {
	SDL_Rect boundry = Environment::get().getResourceManager()->getMap()->get_rect();
	float scale = Environment::get().getWindow()->getCamera()->get_scale();
	double x = (mouse_x / scale) + Environment::get().getWindow()->getCamera()->get_x();
	double y = (mouse_y / scale) + Environment::get().getWindow()->getCamera()->get_y();

	if (x < 0 || x > boundry.x + boundry.w || y < 0 || y > boundry.y + boundry.h) {
		return false;
	}

	int index = int(x / TILE_WIDTH) + ((int(y / TILE_HEIGHT) * int(boundry.w / TILE_WIDTH)));

	if (_selection.type == TYPE_TILE) {
		Environment::get().getResourceManager()->editMap(index, _selection.id);
	}
	return true;
}