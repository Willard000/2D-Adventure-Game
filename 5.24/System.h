#include <memory>
#include <windows.h>

#include "Window.h"
#include "Clock.h"

#ifndef SYSTEM_H
#define SYSTEM_H

#define FILE_CONSOLE_X "iconsole_x"
#define FILE_CONSOLE_Y "iconsole_y"
#define FILE_CONSOLE_W "iconsole_w"
#define FILE_CONSOLE_H "iconsole_h"
#define FILE_CONSOLE_SHOW "bconsole_show"

#define FILE_WINDOW_TITLE "swindow_title"
#define FILE_WINDOW_X "iwindow_x"
#define FILE_WINDOW_Y "iwindow_y"
#define FILE_WINDOW_W "iwindow_w"
#define FILE_WINDOW_H "iwindow_h"
#define FILE_WINDOW_COLOR "window_color"
#define FILE_CAMERA_SPEED "dcamera_speed"

extern const char *SYSTEM_BASE_PATH;
extern const std::string WINDOW_TITLE;

class System {
public:
	System();
	~System();

	void toggleConsole();

	bool isFocused() { return SDL_GetMouseFocus() == _window->getWindow(); }

	std::shared_ptr<Window> getWindow() { return _window; }
private:
	void load_console_settings(FileReader &file);
	void load_window_settings(FileReader &file);
private:
	const int _CONSOLE_X = -1100, _CONSOLE_Y = 450, _CONSOLE_W = 1100, _CONSOLE_H = 600;
	const int _WINDOW_X = 150, _WINDOW_Y = 100, _WINDOW_W = 1600, _WINDOW_H = 900;
	const SDL_Color _WINDOW_COLOR = { 185, 255, 255, 255 };
	const double _CAMERA_SPEED = 390.0;

	HWND _console;
	bool _showConsole;

	std::shared_ptr<Window> _window;

};

#endif