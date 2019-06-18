#include "UIManager.h"

#include "Environment.h"
#include "WindowManager.h"

#include "UIHandler_Editor.h"

#include "Collision.h"

#define MOUSE_WIDTH 1
#define MOUSE_HEIGHT 1

UIManager::UIManager(bool editor) :
	_uiHandler		( editor ? new UIHandler_Editor() : nullptr )
{}

UIManager::~UIManager() {
	delete _uiHandler;
}

void UIManager::update() {

}

void UIManager::check(const int &mouse_x, const int &mouse_y) {
	for (auto it = _uiHandler->_buttons.begin(); it != _uiHandler->_buttons.end(); it++) {
		if (collision((*it)->getRect(), { mouse_x, mouse_y, MOUSE_WIDTH, MOUSE_HEIGHT })) {
			(*it)->execute();
		}
	}
}

void UIManager::render() {
	for (auto it = _uiHandler->_buttons.begin(); it != _uiHandler->_buttons.end(); it++) {
		Environment::get().getWindowManager()->getRenderer()->drawRect((*it)->getRect(), (*it)->getColor());
	}
}