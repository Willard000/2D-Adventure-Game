#include <memory>

#include "Environment.h"

#ifndef ENGINE_H
#define ENGINE_H

class Engine {
public:
	Engine();

	void run();

private:
	void buildEnvironment();
private:
	Environment _environment;

	bool _isRunning;
};

#endif