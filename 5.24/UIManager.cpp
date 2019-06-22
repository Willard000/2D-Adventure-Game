#include "UIManager.h"

#include "Environment.h"
#include "WindowManager.h"
#include "LogManager.h"

#include "UIHandler_Editor.h"

#include "Collision.h"

#define MOUSE_WIDTH 1
#define MOUSE_HEIGHT 1

#define CONFIRM_BUTTON "Yes"
#define DENY_BUTTON "No"

UIManager::UIManager(bool editor) :
	_state			( STATE_IDLE ),
	_current_text	(  ),
	_uiHandler		( editor ? new UIHandler_Editor() : nullptr )
{
	add_button(static_cast<UIHandler_Editor *>(_uiHandler), &UIHandler_Editor::button_new_map, "New");
}

UIManager::~UIManager() {
	delete _uiHandler;
}

void UIManager::remove_button(std::string key) {
	if (_uiHandler->_buttons[key]) {
		delete _uiHandler->_buttons[key];
		_uiHandler->_buttons.erase(key);
		return;
	}

	Environment::get().getLogManager()->log("Couldn't remove button at key - " + key);
}

int UIManager::update() {
	if (_state == STATE_CONFIRM) {
		pop_confirmation();
		if (_on_confirm) {
			_state = STATE_WAITING;
			_on_confirm();
		}
		_on_confirm = nullptr;
		_on_deny = nullptr;
		_state = STATE_IDLE;
	}
	else if (_state == STATE_DENY) {
		pop_confirmation();
		if (_on_deny) {
			_state = STATE_WAITING;
			_on_deny();
		}
		_on_confirm = nullptr;
		_on_deny = nullptr;
		_state = STATE_IDLE;
	}

	return _state;
}

void UIManager::check(const int &mouse_x, const int &mouse_y) {
	for (auto it = _uiHandler->_buttons.begin(); it != _uiHandler->_buttons.end(); it++) {
		if (collision(it->second->getRect(), { mouse_x, mouse_y, MOUSE_WIDTH, MOUSE_HEIGHT })) {
			it->second->execute();
		}
	}
}

void UIManager::render() {
	for (auto it = _uiHandler->_buttons.begin(); it != _uiHandler->_buttons.end(); it++) {
		Environment::get().getWindowManager()->getRenderer()->drawRect(it->second->getRect(), it->second->getColor());
		Environment::get().getWindowManager()->getRenderer()->drawText(it->second->getText(), true);
	}
	if(_state == STATE_WAITING)
		Environment::get().getWindowManager()->getRenderer()->drawText(_current_text, true);
}

void UIManager::set_state(int flag) {
	_state = flag;
}

void UIManager::set_current_text(std::string text) {
	int width_half = Environment::get().getWindowManager()->getWindow()->getWidthHalf();
	int height_half = Environment::get().getWindowManager()->getWindow()->getHeightHalf();
	_current_text = Text (text, { 255, 255, 255, 255 }, 24, 0, width_half, height_half);
}

void UIManager::push_confirmation(Callback on_confirm, Callback on_deny) {
	_on_confirm = on_confirm;
	_on_deny = on_deny;
	int width = Environment::get().getWindowManager()->getWindow()->getWidth();
	int height = Environment::get().getWindowManager()->getWindow()->getHeight();
	add_button(_uiHandler, &UIHandler::confirm_yes, CONFIRM_BUTTON, { (width / 2) - 260, (height / 2) - 60, 120, 30 }, 24);
	add_button(_uiHandler, &UIHandler::confirm_no, DENY_BUTTON, { (width / 2) + 140, (height / 2) - 60, 120, 30 }, 24);
	_state = STATE_WAITING;
}

void UIManager::pop_confirmation() {
	_state = STATE_IDLE;
	remove_button(CONFIRM_BUTTON);
	remove_button(DENY_BUTTON);
}