#include "Window.h"

#include <sstream>

Window::Window(Console_Settings console, Window_Settings window) :
	_width						( window.w ),
	_height						( window.h ),
	_width_half					( window.w / 2 ),
	_height_half				( window.h / 2 ),
	_window(SDL_CreateWindow	( window.title.c_str(), window.x, window.y, window.w, window.h, NULL) ),
	_camera						( new Camera(window.w, window.h) ),
	_renderer					( new Renderer(_window, window.color, _camera) )
{
	_console = GetConsoleWindow();
	MoveWindow(_console, console.x, console.y, console.w, console.h, TRUE);
	ShowWindow(_console, console.show);
	set_window_mode(window.mode);
}

Window::~Window() {
	delete _renderer;
	delete _camera;
	SDL_DestroyWindow(_window);
}

void Window::toggle_console() {
	_show_console = !_show_console;
	ShowWindow(_console, _show_console);
}

void Window::set_window_mode(int window_mode) {
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
	else if (window_mode == 2) {
		SDL_SetWindowBordered(_window, SDL_FALSE);
		SDL_SetWindowSize(_window, mode.w, mode.h);
		SDL_SetWindowPosition(_window, 0, 0);
		_width = mode.w;
		_height = mode.h;
		_width_half = _width / 2;
		_height_half = _height / 2;
	}
	// Borderless
	else if (window_mode == 3) {
		SDL_SetWindowBordered(_window, SDL_FALSE);
	}
	// Fake Fullscreen
	else if (window_mode == 4) {
		SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	// Resizeable
	else if (window_mode == 5) {
		SDL_SetWindowResizable(_window, SDL_TRUE);
		// update window width  & height on resize
	}
}

void Window::print_display_settings() {
	SDL_DisplayMode mode;
	SDL_GetWindowDisplayMode(_window, &mode);
	printf("Window Display Mode\nformat: %d\nrefresh_rate: %d\nwidth: %d\nheight: %d\n", mode.format, mode.refresh_rate, mode.w, mode.h);
}

void Window::update_size() {
	SDL_GetWindowSize(_window, &_width, &_height);
	_camera->set_size(_width, _height);
}

void Window::set_size(int width, int height) {
	SDL_SetWindowSize(_window, width, height);
	_width = width;
	_height = height;
	_camera->set_size(width, height);
}