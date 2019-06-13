#include "Window.h"

Window::Window(
	std::string title,
	int x, int y, int w, int h,
	SDL_Color backgroundColor,
	double camera_speed
) {
	_width = w;
	_height = h;
	_width_half = _width / 2;
	_height_half = _height / 2;
	_window = SDL_CreateWindow(title.c_str(), x, y, w, h, NULL);
	SDL_Rect background = { 0, 0, w, h };
	_camera = new Camera();
	_renderer = new Renderer(_window, _camera, background, backgroundColor);
};

Window::~Window() {
	delete _renderer;
	delete _camera;
	SDL_DestroyWindow(_window);
}