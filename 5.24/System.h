#include <memory>
#include <windows.h>

#include "Window.h"
#include "Clock.h"

#ifndef SYSTEM_H
#define SYSTEM_H

extern const std::string WINDOW_TITLE;

class System {
public:
	System(bool showConsole);
	~System();

	bool isFocused() { return SDL_GetMouseFocus() == _window->getWindow(); }

	std::shared_ptr<Window> getWindow() { return _window; }
private:
	const int _CONSOLE_X = -1100, _CONSOLE_Y = 450, _CONSOLE_W = 1100, _CONSOLE_H = 600;
	const int _WINDOW_X = 150, _WINDOW_Y = 100, _WINDOW_W = 1600, _WINDOW_H = 900;
	const SDL_Color _WINDOW_COLOR = { 185, 255, 255, 255 };

	HWND _console;
	bool _showConsole;

	std::shared_ptr<Window> _window;

};

#endif