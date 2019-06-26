#include "Environment.h"

#include "Texture.h"

#ifndef EDITOR_H
#define EDITOR_H

class Editor {
public:
	Editor();

	void run();

	void draw_lines();
private:
	void build_environment();
	void load_buttons();

	SDL_Texture *makeEditorLineBackground(const int &width, const int &height, const int &tile_width, const int &tile_height, const SDL_Color &color);
private:
	Environment _environment;

	Texture *_lineBackground;

	bool _isRunning;
};

#endif