#include "Environment.h"

#include "Texture.h"

#ifndef EDITOR_H
#define EDITOR_H

class Editor {
public:
	Editor();

	void run();

	void drawLines();
private:
	void buildEnvironment();
private:
	Environment _environment;

	Texture *_lineBackground;

	bool _isRunning;
};

#endif