#include "Window.h"

Window::Window(
	std::string title,
	int x, int y, int w, int h,
	SDL_Color backgroundColor,
	bool camera_locked)
	:
	_width				( w ),
	_height				( h ),
	_width_half			( w / 2 ),
	_height_half		( h / 2 ),
	_window				( SDL_CreateWindow(title.c_str(), x, y, w, h, NULL) ),
	_camera				( new Camera(camera_locked) ),
	_renderer			( new Renderer(_window, _camera, { 0, 0, w, h }, backgroundColor) )
{}

Window::~Window() {
	delete _renderer;
	delete _camera;
	SDL_DestroyWindow(_window);
}