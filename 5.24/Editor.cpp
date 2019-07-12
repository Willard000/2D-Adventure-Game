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

#define BUTTON_NEW_POSITION {0, 0, 80, 20}
#define BUTTON_LOAD_POSITION {0, 20, 80, 20}
#define BUTTON_SAVE_POSITION {0, 40, 80, 20}

#define BUTTON_TILES_POSITION {700, 0, 80, 20}
#define BUTTON_SOLIDS_POSITION {780, 0, 80, 20}
#define BUTTON_OBJECTS_POSITION {860, 0, 80, 20}
#define BUTTON_WARPS_POSITION {940, 0, 80, 20}

#define BUTTON_SELECT_FREE {620, 0, 80, 20}

#define BUTTON_CENTER_PLACEMENT { 80, 0, 80, 20}

Editor::Editor() :
	_isRunning		( true )
{
	build_environment();

	load_buttons();

	_environment.get().get_resource_manager()->load_map(101);
}

void Editor::run() {
	_environment.get_log()->print("Editor Main Loop\n");


	while (_isRunning) {
		_environment.get_window()->get_renderer()->clear();

		_isRunning = _environment.get_input_manager()->get();

		_environment.get_input_manager()->update_editor();

		_environment.get_ui_manager()->update();

		_environment.get_resource_manager()->render();

		_environment.get_ui_manager()->render();

		_environment.get_window()->get_renderer()->render();

		if (_environment.get_clock()->update()) {
			std::string title = "Editor      Map: " + std::to_string(_environment.get_resource_manager()->get_map()->get_id()) + "     " +
				_environment.get_clock()->get_display_time() + "    " + std::to_string(_environment.get_clock()->get_fms());
			_environment.get_window()->set_title(title);
		}
	}

	_environment.get_log()->print("Shutting Down Editor\n");
	_environment.shutdown();
}

void Editor::build_environment() {
	_environment.set_mode(MODE_EDITOR);

	Clock *clock = new Clock();
	_environment.set_clock(clock);

	Log *log = new Log();
	_environment.set_log(log);

	Window *window = load_window();
	_environment.set_window(window);

	ScriptManager *script_manager = new ScriptManager();
	_environment.set_script_manager(script_manager);

	ResourceManager *resource_manager = new ResourceManager();
	_environment.set_resource_manager(resource_manager);

	InputManager *input_manager = new InputManager();
	_environment.set_input_manager(input_manager);

	UIManager *ui_manager = new UIManager();
	_environment.set_ui_manager(ui_manager);
}

void Editor::load_buttons() {
	UIManager* ui_manager = Environment::get().get_ui_manager();

	ui_manager->add_button(new UI::Button_Pressable(&UI::button_new_map, "New", BUTTON_NEW_POSITION));
	ui_manager->add_button(new UI::Button_Pressable(&UI::button_save_map, "Save", BUTTON_SAVE_POSITION));
	ui_manager->add_button(new UI::Button_Pressable(&UI::button_load_map, "Load", BUTTON_LOAD_POSITION));

	ui_manager->add_button(new UI::Button_Pressable(&UI::button_select_tiles, "Tiles", BUTTON_TILES_POSITION));
	ui_manager->add_button(new UI::Button_Pressable(&UI::button_select_solids, "Solids", BUTTON_SOLIDS_POSITION));
	ui_manager->add_button(new UI::Button_Pressable(&UI::button_select_objects, "Objects", BUTTON_OBJECTS_POSITION));
	ui_manager->add_button(new UI::Button_Pressable(&UI::button_select_warps, "Warps", BUTTON_WARPS_POSITION));

	ui_manager->add_button(new UI::Button_Pressable(&UI::button_select_free, "Select", BUTTON_SELECT_FREE));

	ui_manager->add_button(new UI::Button_Pressable(&UI::button_center_placement, "Center", BUTTON_CENTER_PLACEMENT));
}