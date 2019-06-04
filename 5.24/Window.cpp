#include "Window.h"

Window::Window(
	std::string title,
	int x, int y, int w, int h,
	SDL_Color backgroundColor
) {
	_window = SDL_CreateWindow(title.c_str(), x, y, w, h, NULL);
	SDL_Rect background = { 0, 0, w, h };
	_renderer = std::make_shared<Renderer>(_window, background, backgroundColor);
};

Window::~Window() {
	SDL_DestroyWindow(_window);
}