#include "Engine.h"

#include "Environment.h"
#include "Clock.h"
#include "WindowManager.h"
#include "ScriptManager.h"
#include "ResourceManager.h"
#include "InputManager.h"

Engine::Engine() {
	buildEnvironment();

	_isRunning = true;
}

void Engine::run() {
	while (_isRunning) {
		_isRunning = _environment.getInputManager()->get();

		_environment.getInputManager()->update();

		_environment.getScriptManager()->run("Data/Lua/test.lua");

		_environment.getResourceManager()->update();

		_environment.getResourceManager()->render();

		if (_environment.getClock()->update()) {
			_environment.getWindowManager()->updateWindowTitle();
		}
	}
}

void Engine::buildEnvironment() {
	Clock *clock = new Clock();
	_environment.setClock(clock);

	WindowManager *windowManager = new WindowManager();
	_environment.setWindowManager(windowManager);
	
	ScriptManager *scriptManager = new ScriptManager();
	_environment.setScriptManager(scriptManager);

	ResourceManager *resourceManager = new ResourceManager();
	_environment.setResourceManager(resourceManager);

	InputManager *inputManager = new InputManager();
	_environment.setInputManager(inputManager);

}