#include "WindowLoader.h"

#include <sstream>

#include "FileReader.h"

#include "Globals.h"

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
#define FILE_WINDOW_MODE "iwindow_mode"
#define FILE_WINDOW_COLOR "window_color"

Window::Console_Settings load_console_settings(FileReader &file) {
	Window::Console_Settings console;

	if (file.exists(FILE_CONSOLE_X)) console.x = file.get_int(FILE_CONSOLE_X);
	if (file.exists(FILE_CONSOLE_Y)) console.y = file.get_int(FILE_CONSOLE_Y);
	if (file.exists(FILE_CONSOLE_W)) console.w = file.get_int(FILE_CONSOLE_W);
	if (file.exists(FILE_CONSOLE_H)) console.h = file.get_int(FILE_CONSOLE_H);
	if (file.exists(FILE_CONSOLE_SHOW)) console.show = file.get_bool(FILE_CONSOLE_SHOW);

	return console;
}

Window::Window_Settings load_window_settings(FileReader &file) {
	Window::Window_Settings window;

	if (file.exists(FILE_WINDOW_TITLE)) window.title = file.get_string(FILE_WINDOW_TITLE);
	if (file.exists(FILE_WINDOW_X)) window.x = file.get_int(FILE_WINDOW_X);
	if (file.exists(FILE_WINDOW_Y)) window.y = file.get_int(FILE_WINDOW_Y);
	if (file.exists(FILE_WINDOW_W)) window.w = file.get_int(FILE_WINDOW_W);
	if (file.exists(FILE_WINDOW_H)) window.h = file.get_int(FILE_WINDOW_H);
	if (file.exists(FILE_WINDOW_MODE)) window.mode = file.get_int(FILE_WINDOW_MODE);
	if (file.exists(FILE_WINDOW_COLOR)) {
		std::istringstream stream(file.get_string(FILE_WINDOW_COLOR));
		int red, green, blue, alpha;
		stream >> red >> green >> blue >> alpha;
		window.color.r = red;
		window.color.g = green;
		window.color.b = blue;
		window.color.a = alpha;
	}

	return window;
}

Window *load_window() {
	FileReader file(SYSTEM_FILE);
	Window::Console_Settings console = load_console_settings(file);
	Window::Window_Settings window = load_window_settings(file);

	return new Window(console, window);
}