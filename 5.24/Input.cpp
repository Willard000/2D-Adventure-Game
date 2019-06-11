#include "Input.h"

Input::Input(std::shared_ptr<System> system, std::shared_ptr<Clock> clock, std::shared_ptr<InputHandler> inputHandler, Player *player) {
	_system = system;
	_clock = clock;
	_inputHandler = inputHandler;
	_player = player;
}

bool Input::get() {
	SDL_Event event;
	_keys = SDL_GetKeyboardState(NULL);
	while (SDL_PollEvent(&event)) {
		if (event.window.event == SDL_WINDOWEVENT_CLOSE || isKey(SDL_SCANCODE_ESCAPE)) {
			return false;
		}
	}
	return true;
}

bool Input::isKey(const SDL_Keycode &key) {
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

bool Input::isHeld(const SDL_Keycode &key) {
	if (_keys[key]) {
		return true;
	}
	return false;
}

bool Input::isMouse(int &mouse_x, int &mouse_y, const unsigned int &button) {
	if (SDL_GetMouseState(&mouse_x, &mouse_y) & SDL_BUTTON(button) && _system->isFocused()) {
		if (!_mouse_map[button]) {
			_mouse_map[button] = true;
			return true;
		}
		return false;
	}
	_mouse_map[button] = false;
	return false;
}

bool Input::isMouseHeld(int &mouse_x, int &mouse_y, const unsigned int &button) {
	if (SDL_GetMouseState(&mouse_y, &mouse_x) & SDL_BUTTON(button) && _system->isFocused()) {
		return true;
	}
	return false;
}

void Input::update() {
	if (isHeld(SDL_SCANCODE_W)) {
		_inputHandler->publish(new Event_MoveEntity(_player, Event::UP, _clock->getTime()));
	}
	if (isHeld(SDL_SCANCODE_S)) {
		_inputHandler->publish(new Event_MoveEntity(_player, Event::DOWN, _clock->getTime()));
	}
	if (isHeld(SDL_SCANCODE_A)) {
		_inputHandler->publish(new Event_MoveEntity(_player, Event::LEFT, _clock->getTime()));
	}
	if (isHeld(SDL_SCANCODE_D)) {
		_inputHandler->publish(new Event_MoveEntity(_player, Event::RIGHT, _clock->getTime()));
	}
	if (isHeld(SDL_SCANCODE_I)) {
		_inputHandler->publish(new Event_MoveCamera(_system->getWindow().get(), Event::UP, _clock->getTime()));
	}
	if (isHeld(SDL_SCANCODE_K)) {
		_inputHandler->publish(new Event_MoveCamera(_system->getWindow().get(), Event::DOWN, _clock->getTime()));
	}
	if (isHeld(SDL_SCANCODE_J)) {
		_inputHandler->publish(new Event_MoveCamera(_system->getWindow().get(), Event::LEFT, _clock->getTime()));
	}
	if (isHeld(SDL_SCANCODE_L)) {
		_inputHandler->publish(new Event_MoveCamera(_system->getWindow().get(), Event::RIGHT, _clock->getTime()));
	}
	if (isKey(SDL_SCANCODE_O)) {
		_system->getWindow()->toggleCamera();
	}
}