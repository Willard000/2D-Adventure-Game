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
	_input_handler		( new InputHandler() ),
	_mouse_x			( 0 ),
	_mouse_y			( 0 ),
	_mouse_x_prev		( 0 ),
	_mouse_y_prev		( 0 ),
	_mouse_wheel		( 0 ),
	_get_text_input		( false ),
	_text_input			( "" )
{
	Environment::get().get_log()->print("Loading Input Manager");
}

InputManager::~InputManager() {
	Environment::get().get_log()->print("Closing Input Manager");
	delete _input_handler;
}

bool InputManager::get() {
	SDL_Event event;
	_keys = SDL_GetKeyboardState(NULL);
	_mouse_wheel = 0;

	while (SDL_PollEvent(&event)) {
		if (event.window.event == SDL_WINDOWEVENT_CLOSE || is_key(SDL_SCANCODE_ESCAPE)) {
			return false;
		}
		if (event.type == SDL_MOUSEWHEEL) {
			_mouse_wheel = event.wheel.y;
		}
		if (_state == STATE_TEXT_INPUT && event.type == SDL_TEXTINPUT) {
			_text_input += event.text.text;
		}
	}
	
	if (_state == STATE_TEXT_INPUT && is_key(SDL_SCANCODE_BACKSPACE)) {
		if (_text_input.size() > 0) {
			_text_input.pop_back();
		}
	}
	if (_state == STATE_TEXT_INPUT && is_key(SDL_SCANCODE_RETURN)) {
		_state = STATE_IDLE;
	}

	return true;
}

bool InputManager::is_key(const SDL_Keycode &key) {
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

bool InputManager::is_held(const SDL_Keycode &key) {
	if (_keys[key]) {
		return true;
	}
	return false;
}

bool InputManager::is_mouse(const unsigned int &button) {
	if (SDL_GetMouseState(&_mouse_x, &_mouse_y) & SDL_BUTTON(button) && Environment::get().get_window()->is_focused()) {
		if (!_mouse_map[button]) {
			_mouse_map[button] = true;
			return true;
		}
		return false;
	}
	_mouse_map[button] = false;
	return false;
}

bool InputManager::is_mouse_held(const unsigned int &button) {
	if (SDL_GetMouseState(&_mouse_x, &_mouse_y) & SDL_BUTTON(button) && Environment::get().get_window()->is_focused()) {
		return true;
	}
	return false;
}

void InputManager::update() {
	if (is_held(SDL_SCANCODE_W)) {
		_input_handler->publish(new Event_MoveEntity(
			Environment::get().get_resource_manager()->get_player(),
			Event::UP)
		);
	}
	if (is_held(SDL_SCANCODE_S)) {
		_input_handler->publish(new Event_MoveEntity(
			Environment::get().get_resource_manager()->get_player(),
			Event::DOWN)
		);
	}
	if (is_held(SDL_SCANCODE_A)) {
		_input_handler->publish(new Event_MoveEntity(
			Environment::get().get_resource_manager()->get_player(),
			Event::LEFT)
		);
	}
	if (is_held(SDL_SCANCODE_D)) {
		_input_handler->publish(new Event_MoveEntity(
			Environment::get().get_resource_manager()->get_player(),
			Event::RIGHT)
		);
	}
	if (is_held(SDL_SCANCODE_I)) {
		_input_handler->publish(new Event_MoveCamera(
			Environment::get().get_window()->get_camera(),
			Event::UP)
		);
	}
	if (is_held(SDL_SCANCODE_K)) {
		_input_handler->publish(new Event_MoveCamera(
			Environment::get().get_window()->get_camera(),
			Event::DOWN)
		);
	}
	if (is_held(SDL_SCANCODE_J)) {
		_input_handler->publish(new Event_MoveCamera(
			Environment::get().get_window()->get_camera(),
			Event::LEFT)
		);
	}
	if (is_held(SDL_SCANCODE_L)) {
		_input_handler->publish(new Event_MoveCamera(
			Environment::get().get_window()->get_camera(),
			Event::RIGHT)
		);
	}
	if (is_key(SDL_SCANCODE_O)) {
		Environment::get().get_window()->get_camera()->toggle();
		Environment::get().get_window()->get_camera()->set_scale(1.0f);
	}

	if (is_mouse(SDL_BUTTON_MIDDLE)) {
		_mouse_x_prev = _mouse_x;
		_mouse_y_prev = _mouse_y;
	}

	if (is_mouse_held(SDL_BUTTON_MIDDLE)) {
		Environment::get().get_window()->get_camera()->update((_mouse_x_prev - _mouse_x), (_mouse_y_prev - _mouse_y));
		_mouse_x_prev = _mouse_x;
		_mouse_y_prev = _mouse_y;
	}

	if (_mouse_wheel > 0 && !Environment::get().get_window()->get_camera()->get_locked()) {
		Environment::get().get_window()->zoom(MOUSE_SCROLL_FACTOR, _mouse_x, _mouse_y);
	}
	else if (_mouse_wheel < 0 && !Environment::get().get_window()->get_camera()->get_locked()) {
		Environment::get().get_window()->zoom(-MOUSE_SCROLL_FACTOR, _mouse_x, _mouse_y);
	}
}

void InputManager::update_editor() {
	if (is_held(SDL_SCANCODE_W)) {
		_input_handler->publish(new Event_MoveCamera(
			Environment::get().get_window()->get_camera(),
			Event::UP)
		);
	}
	if (is_held(SDL_SCANCODE_S)) {
		_input_handler->publish(new Event_MoveCamera(
			Environment::get().get_window()->get_camera(),
			Event::DOWN)
		);
	}
	if (is_held(SDL_SCANCODE_A)) {
		_input_handler->publish(new Event_MoveCamera(
			Environment::get().get_window()->get_camera(),
			Event::LEFT)
		);
	}
	if (is_held(SDL_SCANCODE_D)) {
		_input_handler->publish(new Event_MoveCamera(
			Environment::get().get_window()->get_camera(),
			Event::RIGHT)
		);
	}

	if (is_key(SDL_SCANCODE_O)) {
		Environment::get().get_window()->get_camera()->set_scale(1.0f);
	}

	if (is_key(SDL_SCANCODE_Q)) {
		Environment::get().get_ui_manager()->delete_element();
	}

	static bool clicked_button = false;
	if (is_mouse(SDL_BUTTON_LEFT)) {
		clicked_button = Environment::get().get_ui_manager()->check_buttons();
		if (!clicked_button && Environment::get().get_ui_manager()->get_placement_type() != TYPE_TILE) {
			Environment::get().get_ui_manager()->check_selection();
		}
	}

	if (!clicked_button && Environment::get().get_ui_manager()->get_state() != UI::STATE_WAITING) {
		if (Environment::get().get_ui_manager()->get_placement_type() == TYPE_TILE && is_mouse_held(SDL_BUTTON_LEFT)) {
			Environment::get().get_ui_manager()->check_selection();
		}
	}


	if (is_mouse(SDL_BUTTON_MIDDLE)) {
		_mouse_x_prev = _mouse_x;
		_mouse_y_prev = _mouse_y;
	}

	if (is_mouse_held(SDL_BUTTON_MIDDLE)) {
		Environment::get().get_window()->get_camera()->update((_mouse_x_prev - _mouse_x), (_mouse_y_prev - _mouse_y));
		_mouse_x_prev = _mouse_x;
		_mouse_y_prev = _mouse_y;
	}

	if (_mouse_wheel > 0) {
		Environment::get().get_window()->zoom(MOUSE_SCROLL_FACTOR, _mouse_x, _mouse_y);
	}
	else if (_mouse_wheel < 0) {
		Environment::get().get_window()->zoom(-MOUSE_SCROLL_FACTOR, _mouse_x, _mouse_y);
	}
}

void InputManager::update_editor_camera() {
	if (is_held(SDL_SCANCODE_W)) {
		_input_handler->publish(new Event_MoveCamera(
			Environment::get().get_window()->get_camera(),
			Event::UP)
		);
	}
	if (is_held(SDL_SCANCODE_S)) {
		_input_handler->publish(new Event_MoveCamera(
			Environment::get().get_window()->get_camera(),
			Event::DOWN)
		);
	}
	if (is_held(SDL_SCANCODE_A)) {
		_input_handler->publish(new Event_MoveCamera(
			Environment::get().get_window()->get_camera(),
			Event::LEFT)
		);
	}
	if (is_held(SDL_SCANCODE_D)) {
		_input_handler->publish(new Event_MoveCamera(
			Environment::get().get_window()->get_camera(),
			Event::RIGHT)
		);
	}
	if (is_key(SDL_SCANCODE_O)) {
		Environment::get().get_window()->get_camera()->toggle();
		Environment::get().get_window()->get_camera()->set_scale(1.0f);
	}

	if (is_mouse(SDL_BUTTON_MIDDLE)) {
		_mouse_x_prev = _mouse_x;
		_mouse_y_prev = _mouse_y;
	}

	if (is_mouse_held(SDL_BUTTON_MIDDLE)) {
		Environment::get().get_window()->get_camera()->update((_mouse_x_prev - _mouse_x), (_mouse_y_prev - _mouse_y));
		_mouse_x_prev = _mouse_x;
		_mouse_y_prev = _mouse_y;
	}

	if (_mouse_wheel > 0 && !Environment::get().get_window()->get_camera()->get_locked()) {
		Environment::get().get_window()->zoom(MOUSE_SCROLL_FACTOR, _mouse_x, _mouse_y);
	}
	else if (_mouse_wheel < 0 && !Environment::get().get_window()->get_camera()->get_locked()) {
		Environment::get().get_window()->zoom(-MOUSE_SCROLL_FACTOR, _mouse_x, _mouse_y);
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
	int width = Environment::get().get_window()->get_width_half();
	int height = Environment::get().get_window()->get_height_half() - 100;

	SDL_StartTextInput();

	Text text;

	while ((end_input = get()) && _state == STATE_TEXT_INPUT) {
		Environment::get().get_window()->get_renderer()->clear();

		Environment::get().get_ui_manager()->update();

		Environment::get().get_resource_manager()->render();
		Environment::get().get_ui_manager()->render();

		if (text_size != _text_input.size()) {		// update text when it changes
			text_size = _text_input.size();
			text = Text(_text_input, { 255, 255, 255, 255 }, 24, 3000, width, height);
		}

		Environment::get().get_window()->get_renderer()->draw_text(text, true);

		Environment::get().get_window()->get_renderer()->render();

		if (Environment::get().get_clock()->update()) {
			std::string title = "Input      Map: " + std::to_string(Environment::get().get_resource_manager()->get_map()->get_id()) + "     " +
				Environment::get().get_clock()->get_display_time() + "    " + std::to_string(Environment::get().get_clock()->get_fms());
			Environment::get().get_window()->set_title(title);
		}
	}

	SDL_StopTextInput();

	if (!end_input) {
		_state = STATE_IDLE;
		return "";
	}

	return _text_input;
}