#include "Editor.h"

#include "Clock.h"
#include "Log.h"
#include "ScriptManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Window.h"
#include "UIManager.h"

#include "WindowLoader.h"

#define LINE_COLOR {200, 200, 200, 50}

Editor::Editor() :
	_isRunning		( true ),
	_lineBackground ( new Texture() )
{
	buildEnvironment();
	//_environment.getResourceManager()->loadMap(0);

	_lineBackground->texture = _environment.getWindow()->getRenderer()->makeEditorLineBackground(
		_environment.getResourceManager()->getMap()->getWidth(),
		_environment.getResourceManager()->getMap()->getHeight(),
		TILE_WIDTH,
		TILE_HEIGHT,
		LINE_COLOR
	);

	_lineBackground->rect = { 0, 0, _environment.getResourceManager()->getMap()->getWidth(), _environment.getResourceManager()->getMap()->getHeight()};

}

void Editor::run() {
	_environment.getLog()->print("Editor Main Loop\n");


	while (_isRunning) {
		_environment.getWindow()->getRenderer()->clear();

		_isRunning = _environment.getInputManager()->get();

		_environment.getInputManager()->updateEditor();

		_environment.getUIManager()->update();

		_environment.getResourceManager()->render();

		drawLines();

		_environment.getUIManager()->render();

		_environment.getWindow()->getRenderer()->render();

		if (_environment.getClock()->update()) {
			std::string title = "Editor      Map: " + std::to_string(_environment.getResourceManager()->getMap()->get_id()) + "     " +
				_environment.getClock()->getDisplayTime() + "    " + std::to_string(_environment.getClock()->getFMS());
			_environment.getWindow()->setTitle(title);
		}
	}

	_environment.getLog()->print("Shutting Down Editor\n");
	_environment.shutdown();
}

void Editor::buildEnvironment() {
	_environment.setMode(MODE_EDITOR);

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

	UIManager *uiManager = new UIManager();
	_environment.setUIManager(uiManager);
}

void Editor::drawLines() {
	_environment.get().getWindow()->getRenderer()->render(_lineBackground, _lineBackground->rect);
}