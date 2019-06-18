#include "InputManager.h"

#include "Environment.h"
#include "WindowManager.h"
#include "ResourceManager.h"
#include "LogManager.h"
#include "UIManager.h"

InputManager::InputManager() :
	_inputHandler		( new InputHandler() )
{
	Environment::get().getLogManager()->log("Loading Input Manager");
}

InputManager::~InputManager() {
	Environment::get().getLogManager()->log("Closing Input Manager");
	delete _inputHandler;
}

bool InputManager::get() {
	SDL_Event event;
	_keys = SDL_GetKeyboardState(NULL);
	_mouse_wheel = 0;
	while (SDL_PollEvent(&event)) {
		if (event.window.event == SDL_WINDOWEVENT_CLOSE || isKey(SDL_SCANCODE_ESCAPE)) {
			return false;
		}
		if (event.type == SDL_MOUSEWHEEL) {
			_mouse_wheel = event.wheel.y;
		}
	}
	return true;
}

bool InputManager::isKey(const SDL_Keycode &key) {
	if (_keys[key]) {
		if (!_key_map[key]) {
			_key_map[key] = true;
			return true;
		}
		return false;
	}
	_key_map[key] = false;
	return false;
}

bool InputManager::isHeld(const SDL_Keycode &key) {
	if (_keys[key]) {
		return true;
	}
	return false;
}

bool InputManager::isMouse(const unsigned int &button) {
	if (SDL_GetMouseState(&_mouse_x, &_mouse_y) & SDL_BUTTON(button) && Environment::get().getWindowManager()->getWindow()->isFocused()) {
		if (!_mouse_map[button]) {
			_mouse_map[button] = true;
			return true;
		}
		return false;
	}
	_mouse_map[button] = false;
	return false;
}

bool InputManager::isMouseHeld(const unsigned int &button) {
	if (SDL_GetMouseState(&_mouse_x, &_mouse_y) & SDL_BUTTON(button) && Environment::get().getWindowManager()->getWindow()->isFocused()) {
		return true;
	}
	return false;
}

void InputManager::update() {
	if (isHeld(SDL_SCANCODE_W)) {
		_inputHandler->publish(new Event_MoveEntity(
			Environment::get().getResourceManager()->getPlayer(),
			Event::UP)
		);
	}
	if (isHeld(SDL_SCANCODE_S)) {
		_inputHandler->publish(new Event_MoveEntity(
			Environment::get().getResourceManager()->getPlayer(),
			Event::DOWN)
		);
	}
	if (isHeld(SDL_SCANCODE_A)) {
		_inputHandler->publish(new Event_MoveEntity(
			Environment::get().getResourceManager()->getPlayer(),
			Event::LEFT)
		);
	}
	if (isHeld(SDL_SCANCODE_D)) {
		_inputHandler->publish(new Event_MoveEntity(
			Environment::get().getResourceManager()->getPlayer(),
			Event::RIGHT)
		);
	}
	if (isHeld(SDL_SCANCODE_I)) {
		_inputHandler->publish(new Event_MoveCamera(
			Environment::get().getWindowManager()->getWindow()->getCamera(),
			Event::UP)
		);
	}
	if (isHeld(SDL_SCANCODE_K)) {
		_inputHandler->publish(new Event_MoveCamera(
			Environment::get().getWindowManager()->getWindow()->getCamera(),
			Event::DOWN)
		);
	}
	if (isHeld(SDL_SCANCODE_J)) {
		_inputHandler->publish(new Event_MoveCamera(
			Environment::get().getWindowManager()->getWindow()->getCamera(),
			Event::LEFT)
		);
	}
	if (isHeld(SDL_SCANCODE_L)) {
		_inputHandler->publish(new Event_MoveCamera(
			Environment::get().getWindowManager()->getWindow()->getCamera(),
			Event::RIGHT)
		);
	}
	if (isKey(SDL_SCANCODE_O)) {
		Environment::get().getWindowManager()->getWindow()->getCamera()->toggle();
	}

	if (isMouse(SDL_BUTTON_MIDDLE)) {
		_mouse_x_prev = _mouse_x;
		_mouse_y_prev = _mouse_y;
	}

	if (isMouseHeld(SDL_BUTTON_MIDDLE)) {
		Environment::get().getWindowManager()->getWindow()->getCamera()->update((_mouse_x_prev - _mouse_x), (_mouse_y_prev - _mouse_y));
		_mouse_x_prev = _mouse_x;
		_mouse_y_prev = _mouse_y;
	}

	if (_mouse_wheel > 0 && !Environment::get().getWindowManager()->getWindow()->getCamera()->getLocked()) {
		Environment::get().getWindowManager()->zoom(MOUSE_SCROLL_FACTOR, _mouse_x, _mouse_y);
	}
	else if (_mouse_wheel < 0 && !Environment::get().getWindowManager()->getWindow()->getCamera()->getLocked()) {
		Environment::get().getWindowManager()->zoom(-MOUSE_SCROLL_FACTOR, _mouse_x, _mouse_y);
	}
}

void InputManager::updateEditor() {
	if (isHeld(SDL_SCANCODE_W)) {
		_inputHandler->publish(new Event_MoveCamera(
			Environment::get().getWindowManager()->getWindow()->getCamera(),
			Event::UP)
		);
	}
	if (isHeld(SDL_SCANCODE_S)) {
		_inputHandler->publish(new Event_MoveCamera(
			Environment::get().getWindowManager()->getWindow()->getCamera(),
			Event::DOWN)
		);
	}
	if (isHeld(SDL_SCANCODE_A)) {
		_inputHandler->publish(new Event_MoveCamera(
			Environment::get().getWindowManager()->getWindow()->getCamera(),
			Event::LEFT)
		);
	}
	if (isHeld(SDL_SCANCODE_D)) {
		_inputHandler->publish(new Event_MoveCamera(
			Environment::get().getWindowManager()->getWindow()->getCamera(),
			Event::RIGHT)
		);
	}

	if (isMouse(SDL_BUTTON_LEFT)) {
		Environment::get().getUIManager()->check(_mouse_x, _mouse_y);
	}


	if (isMouse(SDL_BUTTON_MIDDLE)) {
		_mouse_x_prev = _mouse_x;
		_mouse_y_prev = _mouse_y;
	}

	if (isMouseHeld(SDL_BUTTON_MIDDLE)) {
		Environment::get().getWindowManager()->getWindow()->getCamera()->update((_mouse_x_prev - _mouse_x), (_mouse_y_prev - _mouse_y));
		_mouse_x_prev = _mouse_x;
		_mouse_y_prev = _mouse_y;
	}

	if (_mouse_wheel > 0) {
		Environment::get().getWindowManager()->zoom(MOUSE_SCROLL_FACTOR, _mouse_x, _mouse_y);
	}
	else if (_mouse_wheel < 0) {
		Environment::get().getWindowManager()->zoom(-MOUSE_SCROLL_FACTOR, _mouse_x, _mouse_y);
	}
}