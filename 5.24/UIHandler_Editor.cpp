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

	UIManager *ui_manager = Environment::get().getUIManager();
	InputManager *input_manager = Environment::get().getInputManager();
	ResourceManager *resource_manager = Environment::get().getResourceManager();

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

	if (resource_manager->getMap()->create_new(id, name, width, height, base_tile_id)) {
		resource_manager->loadMap(id);
	}
}

void save_map() {
	Environment::get().getResourceManager()->getMap()->save();
}

void load_map() {
	int id;
	Environment::get().getUIManager()->set_current_text("Enter Map id:");
	Environment::get().getInputManager()->get_text_input(&id);
	if (id == -1) {
		return;
	}

	Environment::get().getResourceManager()->loadMap(id);
}

void UI::button_new_map() {
	Environment::get().getUIManager()->set_current_text("Create New Map?");
	Environment::get().getUIManager()->push_confirmation(create_new_map);
}

void UI::button_save_map() {
	Environment::get().getUIManager()->set_current_text("Save Map?");
	Environment::get().getUIManager()->push_confirmation(save_map);
}

void UI::button_load_map() {
	Environment::get().getUIManager()->set_current_text("Load New Map?");
	Environment::get().getUIManager()->push_confirmation(load_map);
}