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

	void toggleConsole();
	void move(int x, int y)				{ SDL_SetWindowPosition(_window, x, y); }
	void hide()							{ SDL_HideWindow(_window); }
	void show()							{ SDL_ShowWindow(_window); }
	void setTitle(std::string title)	{ SDL_SetWindowTitle(_window, title.c_str()); }
	void setWindowMode(int window_mode);

	bool isFocused()					{ return SDL_GetMouseFocus() == _window; }

	void zoom(const float &amount, const int &mouse_x, const int &mouse_y);

	Camera *getCamera()			  { return _camera;		 }
	SDL_Window *getWindow()		  { return _window;          }
	Renderer *getRenderer()		  { return _renderer;        }

	int getWidth() { return _width; }
	int getHeight() { return _height; }
	int getWidthHalf() { return _width_half; }
	int getHeightHalf() { return _height_half; }

	void printDisplaySettings();
private:
	SDL_Window *_window;
	Camera *_camera;
	Renderer *_renderer;

	int _width, _height;
	int _width_half, _height_half;

	HWND _console;
	bool _showConsole;
};

#endif