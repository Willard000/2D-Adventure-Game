#include <string>
#include <memory>

#include <SDL.h>

#include "Renderer.h"
#include "FileReader.h"
#include "Camera.h"

#include "Entity.h"
#include "PositionComponent.h"
#include "MoveableComponent.h"


#ifndef WINDOW_H
#define WINDOW_H

class Window {
public:
	Window();
	Window(
		std::string title,
		int x, int y, int w, int h,
		SDL_Color backgroundColor,
		double camera_speed
	);
	~Window();

	void move(int x, int y) {SDL_SetWindowPosition(_window, x, y);}
	void hide() { SDL_HideWindow(_window); }
	void show() { SDL_ShowWindow(_window); }
	void setTitle(std::string title) { SDL_SetWindowTitle(_window, title.c_str()); }

	void toggleCamera() { _camera.is_locked = !_camera.is_locked; }
	void updateCamera(Entity *entity, const int &map_width, const int &map_height);
	void moveCamera(int dir, double time);

	Camera *getCamera() { return &_camera; }
	SDL_Window *getWindow()							  { return _window;          }
	std::shared_ptr<Renderer> getRenderer()			  { return _renderer;        }
private:
	SDL_Window *_window;
	std::shared_ptr<Renderer> _renderer;

	int _width, _height;
	int _width_half, _height_half;

	Camera _camera;
};

#endif