#include "InputManager.h"

#include "Environment.h"
#include "WindowManager.h"
#include "ResourceManager.h"

InputManager::InputManager() {
	_inputHandler = new InputHandler();
}

InputManager::~InputManager() {
	delete _inputHandler;
}

bool InputManager::get() {
	SDL_Event event;
	_keys = SDL_GetKeyboardState(NULL);
	while (SDL_PollEvent(&event)) {
		if (event.window.event == SDL_WINDOWEVENT_CLOSE || isKey(SDL_SCANCODE_ESCAPE)) {
			return false;
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

bool InputManager::isMouse(int &mouse_x, int &mouse_y, const unsigned int &button) {
	if (SDL_GetMouseState(&mouse_x, &mouse_y) & SDL_BUTTON(button) && Environment::get().getWindowManager()->getWindow()->isFocused()) {
		if (!_mouse_map[button]) {
			_mouse_map[button] = true;
			return true;
		}
		return false;
	}
	_mouse_map[button] = false;
	return false;
}

bool InputManager::isMouseHeld(int &mouse_x, int &mouse_y, const unsigned int &button) {
	if (SDL_GetMouseState(&mouse_y, &mouse_x) & SDL_BUTTON(button) && Environment::get().getWindowManager()->getWindow()->isFocused()) {
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
}

