#include <string>
#include <memory>

#include <SDL.h>

#include "Renderer.h"

#ifndef WINDOW_H
#define WINDOW_H

class Window {
public:
	Window(
		std::string title,
		int x, int y, int w, int h,
		SDL_Color backgroundColor
	);
	~Window();

	void move(int x, int y) {SDL_SetWindowPosition(_window, x, y);}
	void hide() { SDL_HideWindow(_window); }
	void show() { SDL_ShowWindow(_window); }
	void setTitle(std::string title) { SDL_SetWindowTitle(_window, title.c_str()); }

	SDL_Window *getWindow()							  { return _window;          }
	std::shared_ptr<Renderer> getRenderer()			  { return _renderer;        }
private:
	SDL_Window *_window;
	std::shared_ptr<Renderer> _renderer;
};

#endif