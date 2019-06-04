#include <memory>

#include "System.h"
#include "Input.h"
#include "ResourceManager.h"
#include "InputHandler.h"
#include "Renderer.h"
#include "Clock.h"

#ifndef ENGINE_H
#define ENGINE_H

class Entity;

class Engine {
public:
	Engine();

	void run();

	void updateTitle();
private:
	std::shared_ptr<System> _system;
	std::shared_ptr<Clock> _clock;
	std::shared_ptr<Input> _input;
	std::shared_ptr<InputHandler> _inputHandler;
	std::shared_ptr<ResourceManager> _resourceManager;
	bool _isRunning;
};

#endif