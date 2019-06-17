#include <memory>
#include <windows.h>

#include "Window.h"

#include "FileReader.h"

#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

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

class WindowManager {
public:
	WindowManager(bool is_editor);
	~WindowManager();

	void toggleConsole();
	void updateWindowTitle();

	Window *getWindow() { return _window; }
	Renderer *getRenderer() { return _window->getRenderer(); }
private:
	void load_console_settings(FileReader &file);
	void load_window_settings(FileReader &file, bool is_editor);
private:
	const char *_FILE_PATH = "Data/system.txt";
	const char *_WINDOW_TITLE = "...";
	const int _CONSOLE_X = -1100, _CONSOLE_Y = 450, _CONSOLE_W = 1100, _CONSOLE_H = 600;
	const int _WINDOW_X = 150, _WINDOW_Y = 100, _WINDOW_W = 1600, _WINDOW_H = 900;
	const SDL_Color _WINDOW_COLOR = { 185, 255, 255, 255 };

	HWND _console;
	bool _showConsole;

	Window *_window;
};

#endif