#include "UIHandler_Editor.h"

#include "Environment.h"
#include "UIManager.h"
#include "Window.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Log.h"

#include "Button_Pressable.h"

#include "Map.h"

#include <SDL.h>

void create_new_map() {
	int id;
	std::string name;
	int width, height;
	int base_tile_id;

	Environment::get().getUIManager()->set_current_text("Enter Map id:");
	Environment::get().getInputManager()->get_text_input(&id);
	if (id == -1) {
		return;
	}
	
	Environment::get().getUIManager()->set_current_text("Enter Map Name:");
	Environment::get().getInputManager()->get_text_input(&name);

	Environment::get().getUIManager()->set_current_text("Enter Map Width:");
	Environment::get().getInputManager()->get_text_input(&width);
	if (width == -1 || width > MAP_MAX_WIDTH) {
		return;
	}

	Environment::get().getUIManager()->set_current_text("Enter Map Height:");
	Environment::get().getInputManager()->get_text_input(&height);
	if (height == -1 || height > MAP_MAX_HEIGHT) {
		return;
	}

	Environment::get().getUIManager()->set_current_text("Enter Base Tile:");
	Environment::get().getInputManager()->get_text_input(&base_tile_id);
	if (base_tile_id == -1) {
		return;
	}

	Environment::get().getResourceManager()->getMap()->create_new(id, name, width, height, base_tile_id);
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

UIHandler_Editor::UIHandler_Editor(){

}

void UIHandler_Editor::button_new_map() {
	Environment::get().getUIManager()->set_current_text("Create New Map?");
	Environment::get().getUIManager()->push_confirmation(create_new_map);
}

void UIHandler_Editor::button_save_map() {
	Environment::get().getUIManager()->set_current_text("Save Map?");
	Environment::get().getUIManager()->push_confirmation(save_map);
}

void UIHandler_Editor::button_load_map() {
	Environment::get().getUIManager()->set_current_text("Load New Map?");
	Environment::get().getUIManager()->push_confirmation(load_map);
}