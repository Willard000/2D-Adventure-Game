#include "System.h"
 
const std::string WINDOW_TITLE = "...";

System::System(bool showConsole) {
	_showConsole = showConsole;
	_console = GetConsoleWindow();
	MoveWindow(_console, _CONSOLE_X, _CONSOLE_Y, _CONSOLE_W, _CONSOLE_H, TRUE);
	ShowWindow(_console, _showConsole);

	_window = std::make_shared<Window>(
		WINDOW_TITLE,
		_WINDOW_X, _WINDOW_Y, _WINDOW_W, _WINDOW_H,
		_WINDOW_COLOR
		);
}

System::~System() {

}