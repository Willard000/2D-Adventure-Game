#include "Editor.h"

#include "Clock.h"
#include "LogManager.h"
#include "ScriptManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "WindowManager.h"

Editor::Editor() :
	_isRunning		( true )
{
	buildEnvironment();

	_environment.getResourceManager()->loadMap(0);
}

void Editor::run() {

	while (_isRunning) {
		_isRunning = _environment.getInputManager()->get();

		_environment.getInputManager()->updateEditor();

		_environment.getResourceManager()->render();

		if (_environment.getClock()->update()) {
			_environment.getWindowManager()->updateWindowTitle();
		}
	}
}

void Editor::buildEnvironment() {
	Clock *clock = new Clock();
	_environment.setClock(clock);

	LogManager *logManager = new LogManager();
	_environment.setLogManager(logManager);

	WindowManager *windowManager = new WindowManager(true);
	_environment.setWindowManager(windowManager);

	ScriptManager *scriptManager = new ScriptManager();
	_environment.setScriptManager(scriptManager);

	ResourceManager *resourceManager = new ResourceManager();
	_environment.setResourceManager(resourceManager);

	InputManager *inputManager = new InputManager();
	_environment.setInputManager(inputManager);
}