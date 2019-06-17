#include <string>
#include <memory>

#include <SDL.h>

#include "Renderer.h"
#include "Camera.h"

#ifndef WINDOW_H
#define WINDOW_H

class Window {
public:
	Window();
	Window(
		std::string title,
		int x, int y, int w, int h,
		SDL_Color backgroundColor,
		bool camera_locked
	);
	~Window();

	void move(int x, int y) {SDL_SetWindowPosition(_window, x, y);}
	void hide() { SDL_HideWindow(_window); }
	void show() { SDL_ShowWindow(_window); }
	void setTitle(std::string title) { SDL_SetWindowTitle(_window, title.c_str()); }

	bool isFocused() { return SDL_GetMouseFocus() == _window; }

	Camera *getCamera()			  { return _camera;		 }
	SDL_Window *getWindow()		  { return _window;          }
	Renderer *getRenderer()		  { return _renderer;        }

	int getWidth() { return _width; }
	int getHeight() { return _height; }
	int getWidthHalf() { return _width_half; }
	int getHeightHalf() { return _height_half; }
private:
	SDL_Window *_window;
	Camera *_camera;
	Renderer *_renderer;

	int _width, _height;
	int _width_half, _height_half;
};

#endif