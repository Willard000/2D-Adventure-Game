#include <map>
#include <memory>

#include <SDL.h>

#include "InputHandler.h"

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#define EDITOR_MOUSE_SCROLL_FACTOR 0.05

class InputManager {
public:
	InputManager();
	~InputManager();

	bool get();
	bool isKey(const SDL_Keycode &key);
	bool isHeld(const SDL_Keycode &key);
	bool isMouse(int &mouse_x, int &mouse_y, const unsigned int &button);
	bool isMouseHeld(int &mouse_x, int &mouse_y, const unsigned int &button);

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