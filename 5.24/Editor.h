#include "Environment.h"

#ifndef EDITOR_H
#define EDITOR_H

class Editor {
public:
	Editor();

	void run();
private:
	void build_environment();
	void load_buttons();
private:
	Environment _environment;

	bool _isRunning;
};

#endif