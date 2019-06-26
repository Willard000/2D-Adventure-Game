#include "Window.h"

#include <sstream>

#define CAMERA_ZOOM_SPEED 1

Window::Window(Console_Settings console, Window_Settings window) :
	_width						( window.w ),
	_height						( window.h ),
	_width_half					( window.w / 2 ),
	_height_half				( window.h / 2 ),
	_window(SDL_CreateWindow	( window.title.c_str(), window.x, window.y, window.w, window.h, NULL) ),
	_camera						( new Camera() ),
	_renderer					( new Renderer(_window, window.color, _camera) )
{
	_console = GetConsoleWindow();
	MoveWindow(_console, console.x, console.y, console.w, console.h, TRUE);
	ShowWindow(_console, console.show);
	setWindowMode(window.mode);
}

Window::~Window() {
	delete _renderer;
	delete _camera;
	SDL_DestroyWindow(_window);
}

void Window::toggleConsole() {
	_showConsole = !_showConsole;
	ShowWindow(_console, _showConsole);
}

void Window::zoom(const float &amount, const int &mouse_x, const int &mouse_y) {
	_camera->scale(amount);
	if (amount < 0) {
		_camera->update(-CAMERA_ZOOM_SPEED, -CAMERA_ZOOM_SPEED);
	}
	else {
		_camera->update(CAMERA_ZOOM_SPEED, CAMERA_ZOOM_SPEED);
	}
}

void Window::setWindowMode(int window_mode) {
	SDL_DisplayMode mode;
	SDL_GetDesktopDisplayMode(0, &mode);
	SDL_SetWindowPosition(_window, (mode.w - _width) / 2, (mode.h - _height) / 2);

	if (window_mode) {
		SDL_SetWindowResizable(_window, SDL_FALSE);
	}
	// Fullscreen
	if (window_mode == 1) {
		SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN);
	}
	// Fullscreen Borderless Windowed
	if (window_mode == 2) {
		SDL_SetWindowBordered(_window, SDL_FALSE);
		SDL_SetWindowSize(_window, mode.w, mode.h);
		SDL_SetWindowPosition(_window, 0, 0);
		_width = mode.w;
		_height = mode.h;
		_width_half = _width / 2;
		_height_half = _height / 2;
	}
	// Borderless
	if (window_mode == 3) {
		SDL_SetWindowBordered(_window, SDL_FALSE);
	}
	// Fake Fullscreen
	if (window_mode == 4) {
		SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	// Resizeable
	if (window_mode == 5) {
		SDL_SetWindowResizable(_window, SDL_TRUE);
		// update window width  & height on resize
	}
}

void Window::printDisplaySettings() {
	SDL_DisplayMode mode;
	SDL_GetWindowDisplayMode(_window, &mode);
	printf("Window Display Mode\nformat: %d\nrefresh_rate: %d\nwidth: %d\nheight: %d\n", mode.format, mode.refresh_rate, mode.w, mode.h);
}