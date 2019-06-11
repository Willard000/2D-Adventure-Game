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
	_renderer = std::make_shared<Renderer>(_window, &_camera, background, backgroundColor);
	_camera.speed = camera_speed;
};

Window::~Window() {
	SDL_DestroyWindow(_window);
}

void Window::updateCamera(Entity *entity, const int &map_width, const int &map_height) {
	if (!_camera.is_locked) {
		return;
	}

	PositionComponent *position = GetPosition(entity);
	MoveableComponent *moveable = GetMoveable(entity);

	if (position) {
		_camera.x = position->pos_x - _width_half;
		_camera.y = position->pos_y - _height_half;
	}
	if (moveable) {
		_camera.x = moveable->pos_x - _width_half;
		_camera.y = moveable->pos_y - _height_half;
	}

	if (_camera.x < 0) {
		_camera.x = 0;
	}
	else if (_camera.x > (map_width - _width)) {
		_camera.x = map_width - _width;
	}
	if (_camera.y < 0) {
		_camera.y = 0;
	}
	else if (_camera.y > (map_height - _height)) {
		_camera.y = map_height - _height;
	}
}

void Window::moveCamera(int dir, double time) {
	if (_camera.is_locked) {
		return;
	}

	if (dir == Event::UP) {
		_camera.y -= _camera.speed * time;
	}
	else if (dir == Event::DOWN) {
		_camera.y += _camera.speed * time;
	}
	else if (dir == Event::LEFT) {
		_camera.x -= _camera.speed * time;
	}
	else if (dir == Event::RIGHT) {
		_camera.x += _camera.speed * time;
	}
}