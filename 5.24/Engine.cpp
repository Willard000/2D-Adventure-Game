#include "Engine.h"

#include "Environment.h"
#include "Clock.h"
#include "Log.h"
#include "Window.h"
#include "ScriptManager.h"
#include "ResourceManager.h"
#include "InputManager.h"

#include "WindowLoader.h"

Engine::Engine() :
	_isRunning		( true )
{
	buildEnvironment();
}

void Engine::run() {
	_environment.getLog()->print("Engine Main Loop\n");

	while (_isRunning) {
		_environment.getWindow()->getRenderer()->clear();

		_isRunning = _environment.getInputManager()->get();

		_environment.getInputManager()->update();

		_environment.getScriptManager()->run("Data/Lua/test.lua");

		_environment.getResourceManager()->update();

		_environment.getResourceManager()->render();

		_environment.getWindow()->getRenderer()->render();

		if (_environment.getClock()->update()) {
			std::string title = "Engine      Map: " + std::to_string(_environment.getResourceManager()->getMap()->get_id()) + "     " +
				_environment.getClock()->getDisplayTime() + "    " + std::to_string(_environment.getClock()->getFMS());
			_environment.getWindow()->setTitle(title);
		}
	}

	_environment.getLog()->print("Shutting Down Engine\n");
	_environment.shutdown();
}

void Engine::buildEnvironment() {
	_environment.setMode(MODE_GAME);

	Clock *clock = new Clock();
	_environment.setClock(clock);

	Log *log = new Log();
	_environment.setLog(log);

	Window *window = loadWindow();
	_environment.setWindow(window);
	
	ScriptManager *scriptManager = new ScriptManager();
	_environment.setScriptManager(scriptManager);

	ResourceManager *resourceManager = new ResourceManager();
	_environment.setResourceManager(resourceManager);

	InputManager *inputManager = new InputManager();
	_environment.setInputManager(inputManager);
}