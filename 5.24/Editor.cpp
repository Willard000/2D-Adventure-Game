#include "Editor.h"

#include "Clock.h"
#include "Log.h"
#include "ScriptManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Window.h"
#include "UIManager.h"

#include "UIHandler_Editor.h"
#include "Button_Pressable.h"

#include "WindowLoader.h"

#define LINE_COLOR {200, 200, 200, 50}

#define BUTTON_NEW_POSITION {0, 0, 80, 20}
#define BUTTON_LOAD_POSITION {0, 20, 80, 20}
#define BUTTON_SAVE_POSITION {0, 40, 80, 20}

Editor::Editor() :
	_isRunning		( true ),
	_lineBackground ( new Texture() )
{
	build_environment();
	//_environment.getResourceManager()->loadMap(0);

	_lineBackground->texture = makeEditorLineBackground(
		_environment.getResourceManager()->getMap()->getWidth(),
		_environment.getResourceManager()->getMap()->getHeight(),
		TILE_WIDTH,
		TILE_HEIGHT,
		LINE_COLOR
	);

	_lineBackground->rect = { 0, 0, _environment.getResourceManager()->getMap()->getWidth(), _environment.getResourceManager()->getMap()->getHeight()};

	load_buttons();
}

void Editor::run() {
	_environment.getLog()->print("Editor Main Loop\n");


	while (_isRunning) {
		_environment.getWindow()->getRenderer()->clear();

		_isRunning = _environment.getInputManager()->get();

		_environment.getInputManager()->updateEditor();

		_environment.getUIManager()->update();

		_environment.getResourceManager()->render();

		draw_lines();

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

void Editor::build_environment() {
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

void Editor::draw_lines() {
	_environment.get().getWindow()->getRenderer()->render(_lineBackground, _lineBackground->rect);
}

void Editor::load_buttons() {
	UIManager* ui_manager = Environment::get().getUIManager();

	UI::Button_Pressable *button_new = new UI::Button_Pressable(&UI::button_new_map, "New", BUTTON_NEW_POSITION);
	UI::Button_Pressable *button_save = new UI::Button_Pressable(&UI::button_save_map, "Save", BUTTON_SAVE_POSITION);
	UI::Button_Pressable *button_load = new UI::Button_Pressable(&UI::button_load_map, "Load", BUTTON_LOAD_POSITION);

	ui_manager->add_button(button_new);
	ui_manager->add_button(button_save);
	ui_manager->add_button(button_load);
}

// width - window width
// height - window height
SDL_Texture *Editor::makeEditorLineBackground(const int &width, const int &height, const int &tile_width, const int &tile_height, const SDL_Color &color) {
	SDL_Surface *surface = SDL_CreateRGBSurface(NULL, width, height, RGB_DEPTH, RMASK, GMASK, BMASK, AMASK);

	SDL_Surface *line = SDL_CreateRGBSurface(NULL, 1, height, RGB_DEPTH, RMASK, GMASK, BMASK, AMASK);
	SDL_FillRect(line, NULL, SDL_MapRGB(line->format, color.r, color.g, color.b));

	SDL_Rect pos = { 0, 0, 0,0 };
	for (int i = 0; i <= width; i++) {
		pos.x = int(i * tile_width);
		SDL_BlitSurface(line, NULL, surface, &pos);
	}
	SDL_FreeSurface(line);

	line = SDL_CreateRGBSurface(NULL, width, 1, RGB_DEPTH, RMASK, GMASK, BMASK, AMASK);
	SDL_FillRect(line, NULL, SDL_MapRGB(line->format, color.r, color.g, color.b));

	pos = { 0, 0, 0, 0 };
	for (int i = 0; i <= height; i++) {
		pos.y = int(i * tile_height);
		SDL_BlitSurface(line, NULL, surface, &pos);
	}
	SDL_FreeSurface(line);

	SDL_SetSurfaceAlphaMod(surface, color.a);

	return SDL_CreateTextureFromSurface(_environment.getWindow()->getRenderer()->getRenderer(), surface);
}