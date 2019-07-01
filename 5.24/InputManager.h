#include <map>

#include <SDL.h>

#include "InputHandler.h"

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#define MOUSE_SCROLL_FACTOR 0.05f

class InputManager {
public:
	InputManager();
	~InputManager();

	bool get();
	bool is_key(const SDL_Keycode &key);
	bool is_held(const SDL_Keycode &key);
	bool is_mouse(const unsigned int &button);
	bool is_mouse_held(const unsigned int &button);

	void update();
	void update_editor();

	void get_text_input(int *val);
	void get_text_input(std::string *val);
private:
	std::string start_text_input();
public:
	enum {
		STATE_IDLE = 1,
		STATE_TEXT_INPUT
	};
private:
	InputHandler *_input_handler;

	const Uint8 *_keys;
	std::map<SDL_Keycode, bool> _key_map;
	std::map<unsigned int, bool> _mouse_map;

	int _mouse_x, _mouse_y;
	int _mouse_x_prev, _mouse_y_prev;
	int _mouse_wheel;

	int _state;
	bool _get_text_input;
	std::string _text_input;
};

#endif