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
private:
	Environment _environment;

	bool _isRunning;
};

#endif