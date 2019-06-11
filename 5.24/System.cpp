#include "System.h"
 
extern const char *SYSTEM_BASE_PATH = "Data/system.txt";
const std::string WINDOW_TITLE = "...";

System::System() {
	FileReader file(SYSTEM_BASE_PATH);
	load_console_settings(file);
	load_window_settings(file);
}

System::~System() {

}

void System::toggleConsole() {
	_showConsole = !_showConsole;
	ShowWindow(_console, _showConsole);
}

void System::load_console_settings(FileReader &file) {
	int console_x = _CONSOLE_X, console_y = _CONSOLE_Y;
	int console_w = _CONSOLE_W, console_h = _CONSOLE_H;

	if (file.exists(FILE_CONSOLE_X)) console_x = file.get_int(FILE_CONSOLE_X);
	if (file.exists(FILE_CONSOLE_Y)) console_y = file.get_int(FILE_CONSOLE_Y);
	if (file.exists(FILE_CONSOLE_W)) console_w = file.get_int(FILE_CONSOLE_W);
	if (file.exists(FILE_CONSOLE_H)) console_h = file.get_int(FILE_CONSOLE_H);
	if (file.exists(FILE_CONSOLE_SHOW)) _showConsole = file.get_bool(FILE_CONSOLE_SHOW);

	_console = GetConsoleWindow();
	MoveWindow(_console, console_x, console_y, console_w, console_h, TRUE);
	ShowWindow(_console, _showConsole);
}

void System::load_window_settings(FileReader &file) {
	std::string window_title = WINDOW_TITLE;
	int window_x = _WINDOW_X, window_y = _WINDOW_Y;
	int window_w = _WINDOW_W, window_h = _WINDOW_H;
	SDL_Color window_color = _WINDOW_COLOR;
	double camera_speed = _CAMERA_SPEED;

	if (file.exists(FILE_WINDOW_TITLE)) window_title = file.get_string(FILE_WINDOW_TITLE);
	if (file.exists(FILE_WINDOW_X)) window_x = file.get_int(FILE_WINDOW_X);
	if (file.exists(FILE_WINDOW_Y)) window_y = file.get_int(FILE_WINDOW_Y);
	if (file.exists(FILE_WINDOW_W)) window_w = file.get_int(FILE_WINDOW_W);
	if (file.exists(FILE_WINDOW_H)) window_h = file.get_int(FILE_WINDOW_H);
	if (file.exists(FILE_WINDOW_COLOR)) {
		std::istringstream stream(file.get_string(FILE_WINDOW_COLOR));
		int red, green, blue, alpha;
		stream >> red >> green >> blue >> alpha;
		window_color.r = red;
		window_color.g = green;
		window_color.b = blue;
		window_color.a = alpha;
	}
	if (file.exists(FILE_CAMERA_SPEED)) camera_speed = file.get_double(FILE_CAMERA_SPEED);

	_window = std::make_shared<Window>(
		window_title,
		window_x, window_y,
		window_w, window_h,
		window_color,
		camera_speed
		);
}