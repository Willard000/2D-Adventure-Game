#include "Engine.h"

#include "Environment.h"
#include "Clock.h"
#include "LogManager.h"
#include "WindowManager.h"
#include "ScriptManager.h"
#include "ResourceManager.h"
#include "InputManager.h"

Engine::Engine() :
	_isRunning		( true )
{
	buildEnvironment();
}

void Engine::run() {
	_environment.getLogManager()->log("Engine Main Loop\n");

	while (_isRunning) {
		_environment.getWindowManager()->getRenderer()->clear();

		_isRunning = _environment.getInputManager()->get();

		_environment.getInputManager()->update();

		_environment.getScriptManager()->run("Data/Lua/test.lua");

		_environment.getResourceManager()->update();
		_environment.getResourceManager()->render();

		_environment.getWindowManager()->getWindow()->getRenderer()->render();

		if (_environment.getClock()->update()) {
			_environment.getWindowManager()->updateWindowTitle();
		}
	}

	_environment.getLogManager()->log("Shutting Down Engine\n");
	_environment.shutdown();
}

void Engine::buildEnvironment() {
	Clock *clock = new Clock();
	_environment.setClock(clock);

	LogManager *logManager = new LogManager();
	_environment.setLogManager(logManager);

	WindowManager *windowManager = new WindowManager(false);
	_environment.setWindowManager(windowManager);
	
	ScriptManager *scriptManager = new ScriptManager();
	_environment.setScriptManager(scriptManager);

	ResourceManager *resourceManager = new ResourceManager();
	_environment.setResourceManager(resourceManager);

	InputManager *inputManager = new InputManager();
	_environment.setInputManager(inputManager);
}