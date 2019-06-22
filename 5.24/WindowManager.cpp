#include "WindowManager.h"

#include "Environment.h"
#include "Clock.h"
#include "LogManager.h"

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
#define FILE_CAMERA_SPEED "dcamera_speed"

#define CAMERA_ZOOM_SPEED 1

WindowManager::WindowManager(bool is_editor) :
	_console			( GetConsoleWindow() ),
	_showConsole		( false ),
	_window			    ( nullptr )
{
	Environment::get().getLogManager()->log("Loading Window Manager");

	FileReader file(_FILE_PATH);
	load_console_settings(file);
	load_window_settings(file, is_editor);
}

WindowManager::~WindowManager() {
	Environment::get().getLogManager()->log("Closing Window Manager");

	delete _window;
}

void WindowManager::updateWindowTitle() {
	std::string title = "...      " + Environment::get().getClock()->getDisplayTime() + "    " + std::to_string(Environment::get().getClock()->getFMS());
	_window->setTitle(title);
}

void WindowManager::toggleConsole() {
	_showConsole = !_showConsole;
	ShowWindow(_console, _showConsole);
}

void WindowManager::load_console_settings(FileReader &file) {
	int console_x = _CONSOLE_X, console_y = _CONSOLE_Y;
	int console_w = _CONSOLE_W, console_h = _CONSOLE_H;

	if (file.exists(FILE_CONSOLE_X)) console_x = file.get_int(FILE_CONSOLE_X);
	if (file.exists(FILE_CONSOLE_Y)) console_y = file.get_int(FILE_CONSOLE_Y);
	if (file.exists(FILE_CONSOLE_W)) console_w = file.get_int(FILE_CONSOLE_W);
	if (file.exists(FILE_CONSOLE_H)) console_h = file.get_int(FILE_CONSOLE_H);
	if (file.exists(FILE_CONSOLE_SHOW)) _showConsole = file.get_bool(FILE_CONSOLE_SHOW);

	MoveWindow(_console, console_x, console_y, console_w, console_h, TRUE);
	ShowWindow(_console, _showConsole);
}

void WindowManager::load_window_settings(FileReader &file, bool is_editor) {
	std::string window_title = _WINDOW_TITLE;
	int window_x = _WINDOW_X, window_y = _WINDOW_Y;
	int window_w = _WINDOW_W, window_h = _WINDOW_H;
	int window_mode = 0;
	SDL_Color window_color = _WINDOW_COLOR;

	if (file.exists(FILE_WINDOW_TITLE)) window_title = file.get_string(FILE_WINDOW_TITLE);
	if (file.exists(FILE_WINDOW_X)) window_x = file.get_int(FILE_WINDOW_X);
	if (file.exists(FILE_WINDOW_Y)) window_y = file.get_int(FILE_WINDOW_Y);
	if (file.exists(FILE_WINDOW_W)) window_w = file.get_int(FILE_WINDOW_W);
	if (file.exists(FILE_WINDOW_H)) window_h = file.get_int(FILE_WINDOW_H);
	if (file.exists(FILE_WINDOW_MODE)) window_mode = file.get_int(FILE_WINDOW_MODE);
	if (file.exists(FILE_WINDOW_COLOR)) {
		std::istringstream stream(file.get_string(FILE_WINDOW_COLOR));
		int red, green, blue, alpha;
		stream >> red >> green >> blue >> alpha;
		window_color.r = red;
		window_color.g = green;
		window_color.b = blue;
		window_color.a = alpha;
	}

	_window = new Window(
		window_title,
		window_x, window_y,
		window_w, window_h,
		window_color,
		!is_editor,
		window_mode
		);
}

void WindowManager::zoom(const float &amount, const int &mouse_x, const int &mouse_y) {
	_window->getRenderer()->scale(amount);
	if (amount < 0) {
		_window->getCamera()->update(-CAMERA_ZOOM_SPEED, -CAMERA_ZOOM_SPEED);
	}
	else {
		_window->getCamera()->update(CAMERA_ZOOM_SPEED, CAMERA_ZOOM_SPEED);
	}
}

void WindowManager::printDisplaySettings() {
		SDL_DisplayMode mode;
		SDL_GetWindowDisplayMode(_window->getWindow(), &mode);
		printf("Window Display Mode\nformat: %d\nrefresh_rate: %d\nwidth: %d\nheight: %d\n", mode.format, mode.refresh_rate, mode.w, mode.h);
}