#include <string>
#include <Windows.h>

#include <SDL.h>

#include "Renderer.h"
#include "Camera.h"
#include "FileReader.h"

#ifndef WINDOW_H
#define WINDOW_H

class Window {
public:
	struct Console_Settings {
		int x = -1100, y = 450, w = 1100, h = 600;
		bool show = true;
	};

	struct Window_Settings {
		std::string title = "...";
		int x = 150, y = 100, w = 1600, h = 900;
		int mode = 0;
		SDL_Color color = { 185, 255, 255, 255 };
	};

	Window(Console_Settings console, Window_Settings window);

	~Window();

	void toggle_console();
	void move(int x, int y)				{ SDL_SetWindowPosition(_window, x, y); }
	void hide()							{ SDL_HideWindow(_window); }
	void show()							{ SDL_ShowWindow(_window); }
	void set_title(std::string title)	{ SDL_SetWindowTitle(_window, title.c_str()); }
	void set_window_mode(int window_mode);

	bool is_focused()					{ return SDL_GetMouseFocus() == _window; }

	void zoom(const float &amount, const int &mouse_x, const int &mouse_y);

	Camera *get_camera()			  { return _camera;			 }
	SDL_Window *get_window()		  { return _window;          }
	Renderer *get_renderer()		  { return _renderer;        }

	int get_width() { return _width; }
	int get_height() { return _height; }
	int get_width_half() { return _width_half; }
	int get_height_half() { return _height_half; }

	void print_display_settings();
private:
	SDL_Window *_window;
	Camera *_camera;
	Renderer *_renderer;

	int _width, _height;
	int _width_half, _height_half;

	HWND _console;
	bool _show_console;
};

#endif