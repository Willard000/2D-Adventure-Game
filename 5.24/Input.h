#include <map>
#include <memory>

#include <SDL.h>

#include "System.h"
#include "Clock.h"
#include "InputHandler.h"
#include "Player.h"

#ifndef INPUT_H
#define INPUT_H

class Input {
public:
	Input(
		std::shared_ptr<System> system,
		std::shared_ptr<Clock> clock,
		std::shared_ptr<InputHandler> inputHandler,
		Player *player);

	bool get();
	bool isKey(const SDL_Keycode &key);
	bool isHeld(const SDL_Keycode &key);
	bool isMouse(int &mouse_x, int &mouse_y, const unsigned int &button);
	bool isMouseHeld(int &mouse_x, int &mouse_y, const unsigned int &button);

	void update();
private:
	std::shared_ptr<System> _system;
	std::shared_ptr<Clock> _clock;
	std::shared_ptr<InputHandler> _inputHandler;
	Player *_player;
	const Uint8 *_keys;
	std::map<SDL_Keycode, bool> _key_map;
	std::map<unsigned int, bool> _mouse_map;
};

#endif