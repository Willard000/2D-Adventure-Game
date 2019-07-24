#include "UIHandler_Editor.h"

#include "Environment.h"
#include "UIManager.h"
#include "Window.h"
#include "InputManager.h"
#include "ResourceManager.h"

#include "Map.h"

void create_new_map() {
	int id;
	std::string name;
	int width, height;
	int base_tile_id;

	UIManager *ui_manager = Environment::get().get_ui_manager();
	InputManager *input_manager = Environment::get().get_input_manager();
	ResourceManager *resource_manager = Environment::get().get_resource_manager();

	ui_manager->set_current_text("Enter Map id:");
	input_manager->get_text_input(&id);
	if (id == -1) {
		return;
	}
	
	ui_manager->set_current_text("Enter Map Name:");
	input_manager->get_text_input(&name);

	ui_manager->set_current_text("Enter Map Width:");
	input_manager->get_text_input(&width);
	if (width == -1 || width > MAP_MAX_WIDTH) {
		return;
	}

	ui_manager->set_current_text("Enter Map Height:");
	input_manager->get_text_input(&height);
	if (height == -1 || height > MAP_MAX_HEIGHT) {
		return;
	}

	ui_manager->set_current_text("Enter Base Tile:");
	input_manager->get_text_input(&base_tile_id);
	if (base_tile_id == -1) {
		return;
	}

	if (resource_manager->get_map()->create_new(id, name, width, height, base_tile_id)) {
		resource_manager->load_map(id);
	}
}

void save_map() {
	Environment::get().get_resource_manager()->get_map()->save();
}

void load_map() {
	int id;
	Environment::get().get_ui_manager()->set_current_text("Enter Map id:");
	Environment::get().get_input_manager()->get_text_input(&id);
	if (id == -1) {
		return;
	}

	Environment::get().get_resource_manager()->load_map(id);
}

void UI::button_new_map() {
	Environment::get().get_ui_manager()->set_current_text("Create New Map?");
	Environment::get().get_ui_manager()->push_confirmation(create_new_map);
}

void UI::button_save_map() {
	Environment::get().get_ui_manager()->set_current_text("Save Map?");
	Environment::get().get_ui_manager()->push_confirmation(save_map);
}

void UI::button_load_map() {
	Environment::get().get_ui_manager()->set_current_text("Load New Map?");
	Environment::get().get_ui_manager()->push_confirmation(load_map);
}

void UI::button_select_tiles() {
	Environment::get().get_ui_manager()->set_state(STATE_PLACING);
	Environment::get().get_ui_manager()->set_selection_type(TYPE_TILE);
}

void UI::button_select_solids() {
	Environment::get().get_ui_manager()->set_state(STATE_PLACING);
	Environment::get().get_ui_manager()->set_selection_type(TYPE_SOLID);
}

void UI::button_select_warps() {
	Environment::get().get_ui_manager()->set_state(STATE_PLACING);
	Environment::get().get_ui_manager()->set_selection_type(TYPE_WARP);
}

void UI::button_select_objects() {
	Environment::get().get_ui_manager()->set_state(STATE_PLACING);
	Environment::get().get_ui_manager()->set_selection_type(TYPE_OBJECT);
}

void UI::button_select_enemies() {
	Environment::get().get_ui_manager()->set_state(STATE_PLACING);
	Environment::get().get_ui_manager()->set_selection_type(TYPE_ENEMY);
}

void UI::button_select_effects() {
	Environment::get().get_ui_manager()->set_state(STATE_PLACING);
	Environment::get().get_ui_manager()->set_selection_type(TYPE_EFFECT);
}

void UI::button_select_free() {
	Environment::get().get_ui_manager()->set_state(STATE_SELECTING);
	Environment::get().get_ui_manager()->set_selection_type(TYPE_SELECT);
}

void UI::button_align_placement() {
	Environment::get().get_ui_manager()->toggle_alignment();
}