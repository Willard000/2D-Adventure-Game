#include "Environment.h"

#ifndef EDITOR_H
#define EDITOR_H

class Editor {
public:
	Editor();

	void run();
private:
	void buildEnvironment();
private:
	Environment _environment;

	bool _isRunning;
};

#endif