#include "UIManager.h"

#include "Environment.h"
#include "ResourceManager.h"
#include "Window.h"
#include "Log.h"

#include "UIHandler_Editor.h"

#include "Collision.h"

#define MOUSE_WIDTH 1
#define MOUSE_HEIGHT 1

#define CONFIRM_BUTTON "Yes"
#define DENY_BUTTON "No"

#define ELEMENT_SELECTION_HEIGHT 300
#define ELEMENT_AREA_COLOR { 100, 100, 100, 100 }

UIManager::UIManager() :
	_state					( STATE_IDLE ),
	_element_area			( { {Environment::get().getWindow()->getWidth() - ELEMENT_AREA_WIDTH, 0, ELEMENT_AREA_WIDTH, Environment::get().getWindow()->getHeight()},
								 ELEMENT_AREA_COLOR,
								{Environment::get().getWindow()->getWidth() - ELEMENT_AREA_WIDTH, ELEMENT_SELECTION_HEIGHT, ELEMENT_AREA_WIDTH, Environment::get().getWindow()->getHeight()},
								{Environment::get().getWindow()->getWidth() - ELEMENT_AREA_WIDTH, 0, ELEMENT_AREA_WIDTH, ELEMENT_SELECTION_HEIGHT} } ),
	_element_selected		( false ),
	_element_type			( TYPE_TILE ),
	_element_index			( -1 ),
	_uiHandler				( Environment::get().getMode() == MODE_EDITOR ? new UIHandler_Editor() : nullptr )
{
	Environment::get().getLog()->print("Loading UI Manager");
	add_button(static_cast<UIHandler_Editor *>(_uiHandler), &UIHandler_Editor::button_new_map, "New");
	add_button(static_cast<UIHandler_Editor *>(_uiHandler), &UIHandler_Editor::button_save_map, "Save", { 0, 20, 80, 20 });
	add_button(static_cast<UIHandler_Editor *>(_uiHandler), &UIHandler_Editor::button_load_map, "Load", { 0, 40, 80, 20 });
}

UIManager::~UIManager() {
	Environment::get().getLog()->print("Closing UI Manager");
	delete _uiHandler;
}

void UIManager::remove_button(std::string key) {
	if (_uiHandler->_buttons[key]) {
		delete _uiHandler->_buttons[key];
		_uiHandler->_buttons.erase(key);
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

bool UIManager::check(const int &mouse_x, const int &mouse_y) {
	for (auto it = _uiHandler->_buttons.begin(); it != _uiHandler->_buttons.end(); it++) {
		if (collision(it->second->getRect(), { mouse_x, mouse_y, MOUSE_WIDTH, MOUSE_HEIGHT })) {
			it->second->execute();
			return true;
		}
	}
	
	if (mouse_x >= _element_area.element_area.x) {
		_element_index = select_element(mouse_x, mouse_y);
		if (_element_index != -1) {
			_element_selected = true;
		}
		else {
			_element_selected = false;
		}
		return true;
	}

	return false;
}

void UIManager::render() {
	for (auto it = _uiHandler->_buttons.begin(); it != _uiHandler->_buttons.end(); it++) {
		Environment::get().getWindow()->getRenderer()->drawRect(it->second->getRect(), it->second->getColor());
		Environment::get().getWindow()->getRenderer()->drawText(it->second->getText(), true);
	}
	if(_state == STATE_WAITING)
		Environment::get().getWindow()->getRenderer()->drawText(_current_text, true);

	Environment::get().getResourceManager()->renderEditor(_element_area);
}

void UIManager::set_state(int flag) {
	_state = flag;
}

void UIManager::set_current_text(std::string text) {
	int width_half = Environment::get().getWindow()->getWidthHalf();
	int height_half = Environment::get().getWindow()->getHeightHalf();
	_current_text = Text (text, { 255, 255, 255, 255 }, 24, 0, width_half, height_half - 200);
}

void UIManager::push_confirmation(Callback on_confirm, Callback on_deny) {
	_on_confirm = on_confirm;
	_on_deny = on_deny;
	int width = Environment::get().getWindow()->getWidth();
	int height = Environment::get().getWindow()->getHeight();
	add_button(_uiHandler, &UIHandler::confirm_yes, CONFIRM_BUTTON, { (width / 2) - 260, (height / 2) - 60, 120, 30 }, 24);
	add_button(_uiHandler, &UIHandler::confirm_no, DENY_BUTTON, { (width / 2) + 140, (height / 2) - 60, 120, 30 }, 24);
	_state = STATE_WAITING;
}

void UIManager::pop_confirmation() {
	remove_button(CONFIRM_BUTTON);
	remove_button(DENY_BUTTON);
}

int UIManager::select_element(const int &mouse_x, const int &mouse_y) {
	if (mouse_y < _element_area.element_area.y) {
		return -1;
	}
	int index = int((mouse_x - _element_area.element_area.x) / ELEMENT_WIDTH) + int(((mouse_y - _element_area.element_area.y) / ELEMENT_HEIGHT) * ELEMENT_ROW_SIZE);
	if (index < 0 || index > (int)Environment::get().getResourceManager()->getSurfaceSize(_element_type)) {
		return -1;
	}
	return index;
}