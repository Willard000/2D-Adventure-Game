#include "Editor.h"

#include "Clock.h"
#include "Log.h"
#include "_Lua.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Window.h"
#include "UIManager.h"

#include "UIHandler_Editor.h"
#include "Button_Pressable.h"

#include "WindowLoader.h"

#define BUTTON_NEW_POSITION {0, 0, 100, 25}
#define BUTTON_LOAD_POSITION {0, 25, 100, 25}
#define BUTTON_SAVE_POSITION {0, 50, 100, 25}

#define BUTTON_TILES_POSITION {1245, 0, 100, 25}
#define BUTTON_OBJECTS_POSITION {1245, 25, 100, 25}
#define BUTTON_ENEMIES_POSITION {1245, 50, 100, 25}
#define BUTTON_EFFECTS_POSITION {1245, 75, 100, 25}
#define BUTTON_ITEMS_POSITION {1245, 100, 100, 25}
#define BUTTON_NPCS_POSITION {1245, 125, 100, 25}
#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 25

#define BUTTON_SELECT_FREE_POSITION {600, 0, 100, 25}
#define BUTTON_ALIGN_PLACEMENT_POSITION {700, 0, 100, 25}
#define BUTTON_STACKING_POSITION {800, 0, 100, 25}
#define BUTTON_SOLIDS_POSITION {900, 0, 100, 25}
#define BUTTON_WARPS_POSITION {1000, 0, 100, 25}

#define BUTTON_ROTATE_POSITION {200, 0, 75, 25}
#define BUTTON_SCALE_POSITION {275, 0, 75, 25}

Editor::Editor() :
	_isRunning		( true )
{
	build_environment();

	load_buttons();

	_environment.get().get_resource_manager()->load_map(1, true);
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

	Lua *lua = new Lua();
	_environment.set_lua(lua);

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

	int x = ui_manager->get_element_area().x;
	int y = ui_manager->get_element_area().y;
	SDL_Rect rect = { x - BUTTON_WIDTH, y, BUTTON_WIDTH, BUTTON_HEIGHT };
	ui_manager->add_button(new UI::Button_Pressable(&UI::button_select_tiles, "Tiles", rect)); rect.y += BUTTON_HEIGHT;
	ui_manager->add_button(new UI::Button_Pressable(&UI::button_select_objects, "Objects", rect)); rect.y += BUTTON_HEIGHT;
	ui_manager->add_button(new UI::Button_Pressable(&UI::button_select_enemies, "Enemies", rect)); rect.y += BUTTON_HEIGHT;
	ui_manager->add_button(new UI::Button_Pressable(&UI::button_select_effects, "Effects", rect)); rect.y += BUTTON_HEIGHT;
	ui_manager->add_button(new UI::Button_Pressable(&UI::button_select_items, "Items", rect)); rect.y += BUTTON_HEIGHT;
	ui_manager->add_button(new UI::Button_Pressable(&UI::button_select_npcs, "NPCS", rect)); rect.y += BUTTON_HEIGHT;

	ui_manager->add_button(new UI::Button_Pressable(&UI::button_select_free, "Select", BUTTON_SELECT_FREE_POSITION));
	ui_manager->add_button(new UI::Button_Pressable(&UI::button_align_placement, "Align", BUTTON_ALIGN_PLACEMENT_POSITION));
	ui_manager->add_button(new UI::Button_Pressable(&UI::button_allow_stacking, "Stacking", BUTTON_STACKING_POSITION));
	ui_manager->add_button(new UI::Button_Pressable(&UI::button_select_solids, "Solids", BUTTON_SOLIDS_POSITION));
	ui_manager->add_button(new UI::Button_Pressable(&UI::button_select_warps, "Warps", BUTTON_WARPS_POSITION));

	ui_manager->add_button(new UI::Button_Pressable(&UI::button_rotate_selection, "Rotate", BUTTON_ROTATE_POSITION));
	ui_manager->add_button(new UI::Button_Pressable(&UI::button_scale_selection, "Scale", BUTTON_SCALE_POSITION));
}