#include "InputManager.h"

#include "Environment.h"
#include "Window.h"
#include "ResourceManager.h"
#include "Log.h"
#include "UIManager.h"
#include "Clock.h"

bool valid_string_to_int(std::string str) {
	if (str == "") {
		return false;
	}

	for (auto it = str.begin(); it != str.end(); it++) {
		int val = int(*it);
		if (val < 48 || val > 57) {
			return false;
		}
	}
	return true;
}

InputManager::InputManager() :
	_state				( STATE_IDLE ),
	_inputHandler		( new InputHandler() ),
	_mouse_x			( 0 ),
	_mouse_y			( 0 ),
	_mouse_x_prev		( 0 ),
	_mouse_y_prev		( 0 ),
	_mouse_wheel		( 0 ),
	_get_text_input		( false ),
	_text_input			( "" )
{
	Environment::get().getLog()->print("Loading Input Manager");
}

InputManager::~InputManager() {
	Environment::get().getLog()->print("Closing Input Manager");
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
		if (_state == STATE_TEXT_INPUT && event.type == SDL_TEXTINPUT) {
			_text_input += event.text.text;
		}
	}
	
	if (_state == STATE_TEXT_INPUT && isKey(SDL_SCANCODE_BACKSPACE)) {
		if (_text_input.size() > 0) {
			_text_input.pop_back();
		}
	}
	if (_state == STATE_TEXT_INPUT && isKey(SDL_SCANCODE_RETURN)) {
		_state = STATE_IDLE;
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
	if (SDL_GetMouseState(&_mouse_x, &_mouse_y) & SDL_BUTTON(button) && Environment::get().getWindow()->isFocused()) {
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
	if (SDL_GetMouseState(&_mouse_x, &_mouse_y) & SDL_BUTTON(button) && Environment::get().getWindow()->isFocused()) {
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
			Environment::get().getWindow()->getCamera(),
			Event::UP)
		);
	}
	if (isHeld(SDL_SCANCODE_K)) {
		_inputHandler->publish(new Event_MoveCamera(
			Environment::get().getWindow()->getCamera(),
			Event::DOWN)
		);
	}
	if (isHeld(SDL_SCANCODE_J)) {
		_inputHandler->publish(new Event_MoveCamera(
			Environment::get().getWindow()->getCamera(),
			Event::LEFT)
		);
	}
	if (isHeld(SDL_SCANCODE_L)) {
		_inputHandler->publish(new Event_MoveCamera(
			Environment::get().getWindow()->getCamera(),
			Event::RIGHT)
		);
	}
	if (isKey(SDL_SCANCODE_O)) {
		Environment::get().getWindow()->getCamera()->toggle();
		Environment::get().getWindow()->getRenderer()->setScale(1.0f);
	}

	if (isMouse(SDL_BUTTON_MIDDLE)) {
		_mouse_x_prev = _mouse_x;
		_mouse_y_prev = _mouse_y;
	}

	if (isMouseHeld(SDL_BUTTON_MIDDLE)) {
		Environment::get().getWindow()->getCamera()->update((_mouse_x_prev - _mouse_x), (_mouse_y_prev - _mouse_y));
		_mouse_x_prev = _mouse_x;
		_mouse_y_prev = _mouse_y;
	}

	if (_mouse_wheel > 0 && !Environment::get().getWindow()->getCamera()->getLocked()) {
		Environment::get().getWindow()->zoom(MOUSE_SCROLL_FACTOR, _mouse_x, _mouse_y);
	}
	else if (_mouse_wheel < 0 && !Environment::get().getWindow()->getCamera()->getLocked()) {
		Environment::get().getWindow()->zoom(-MOUSE_SCROLL_FACTOR, _mouse_x, _mouse_y);
	}
}

void InputManager::updateEditor() {
	if (isHeld(SDL_SCANCODE_W)) {
		_inputHandler->publish(new Event_MoveCamera(
			Environment::get().getWindow()->getCamera(),
			Event::UP)
		);
	}
	if (isHeld(SDL_SCANCODE_S)) {
		_inputHandler->publish(new Event_MoveCamera(
			Environment::get().getWindow()->getCamera(),
			Event::DOWN)
		);
	}
	if (isHeld(SDL_SCANCODE_A)) {
		_inputHandler->publish(new Event_MoveCamera(
			Environment::get().getWindow()->getCamera(),
			Event::LEFT)
		);
	}
	if (isHeld(SDL_SCANCODE_D)) {
		_inputHandler->publish(new Event_MoveCamera(
			Environment::get().getWindow()->getCamera(),
			Event::RIGHT)
		);
	}

	if (isKey(SDL_SCANCODE_O)) {
		Environment::get().getWindow()->getRenderer()->setScale(1.0f);
	}

	if (isMouse(SDL_BUTTON_LEFT)) {
		if (!Environment::get().getUIManager()->check(_mouse_x, _mouse_y)) {
			Environment::get().getResourceManager()->editMap(2, 2);
		}
	}


	if (isMouse(SDL_BUTTON_MIDDLE)) {
		_mouse_x_prev = _mouse_x;
		_mouse_y_prev = _mouse_y;
	}

	if (isMouseHeld(SDL_BUTTON_MIDDLE)) {
		Environment::get().getWindow()->getCamera()->update((_mouse_x_prev - _mouse_x), (_mouse_y_prev - _mouse_y));
		_mouse_x_prev = _mouse_x;
		_mouse_y_prev = _mouse_y;
	}

	if (_mouse_wheel > 0) {
		Environment::get().getWindow()->zoom(MOUSE_SCROLL_FACTOR, _mouse_x, _mouse_y);
	}
	else if (_mouse_wheel < 0) {
		Environment::get().getWindow()->zoom(-MOUSE_SCROLL_FACTOR, _mouse_x, _mouse_y);
	}
}

void InputManager::get_text_input(int *val) {
	std::string input = start_text_input();
	if (valid_string_to_int(input)) {
		*val = std::stoi(input);
	}
	else {
		*val = -1;
	}
}

void InputManager::get_text_input(std::string *val) {
	*val = start_text_input();
}

std::string InputManager::start_text_input() {
	unsigned int text_size = 0;
	bool end_input = false;
	_text_input = "";
	_state = STATE_TEXT_INPUT;
	int width = Environment::get().getWindow()->getWidthHalf();
	int height = Environment::get().getWindow()->getHeightHalf() - 100;

	SDL_StartTextInput();

	Text text;

	while ((end_input = get()) && _state == STATE_TEXT_INPUT) {
		Environment::get().getWindow()->getRenderer()->clear();

		Environment::get().getUIManager()->update();

		Environment::get().getResourceManager()->render();
		Environment::get().getUIManager()->render();

		if (text_size != _text_input.size()) {		// update text when it changes
			text_size = _text_input.size();
			text = Text(_text_input, { 255, 255, 255, 255 }, 24, 3000, width, height);
		}

		Environment::get().getWindow()->getRenderer()->drawText(text, true);

		Environment::get().getWindow()->getRenderer()->render();

		if (Environment::get().getClock()->update()) {
			std::string title = "Input      Map: " + std::to_string(Environment::get().getResourceManager()->getMap()->get_id()) + "     " +
				Environment::get().getClock()->getDisplayTime() + "    " + std::to_string(Environment::get().getClock()->getFMS());
			Environment::get().getWindow()->setTitle(title);
		}
	}

	SDL_StopTextInput();

	if (!end_input) {
		_state = STATE_IDLE;
		return "";
	}

	return _text_input;
}