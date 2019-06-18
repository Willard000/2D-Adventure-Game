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
	bool isKey(const SDL_Keycode &key);
	bool isHeld(const SDL_Keycode &key);
	bool isMouse(const unsigned int &button);
	bool isMouseHeld(const unsigned int &button);

	void update();
	void updateEditor();
private:
	InputHandler *_inputHandler;

	const Uint8 *_keys;
	std::map<SDL_Keycode, bool> _key_map;
	std::map<unsigned int, bool> _mouse_map;

	int _mouse_x, _mouse_y;
	int _mouse_x_prev, _mouse_y_prev;
	int _mouse_wheel;
};

#endif