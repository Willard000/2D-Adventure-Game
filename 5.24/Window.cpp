#include "Window.h"

Window::Window(
	std::string title,
	int x, int y, int w, int h,
	SDL_Color backgroundColor,
	bool camera_locked,
	int window_mode)
	:
	_width				( w ),
	_height				( h ),
	_width_half			( w / 2 ),
	_height_half		( h / 2 ),
	_window				( SDL_CreateWindow(title.c_str(), x, y, w, h, NULL) ),
	_camera				( new Camera(camera_locked) ),
	_renderer			( new Renderer(_window, backgroundColor, _camera) )
{
	setWindowMode(window_mode);
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

Window::~Window() {
	delete _renderer;
	delete _camera;
	SDL_DestroyWindow(_window);
}