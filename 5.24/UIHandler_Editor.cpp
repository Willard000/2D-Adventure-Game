#include "UIHandler_Editor.h"

#include "Environment.h"
#include "UIManager.h"
#include "WindowManager.h"
#include "InputManager.h"

#include "Button_Pressable.h"

#include <iostream>
void create_new_map() {
	std::string name = " ";
	int width = 0, height = 0;
	int base_tile_id = 0;

	Environment::get().getUIManager()->set_current_text("Enter Map Name:");
	name = Environment::get().getInputManager()->start_text_input();
	Environment::get().getUIManager()->set_current_text("Enter Map Width:");
	width = std::stoi(Environment::get().getInputManager()->start_text_input());
	Environment::get().getUIManager()->set_current_text("Enter Map Height:");
	height = std::stoi(Environment::get().getInputManager()->start_text_input());
	Environment::get().getUIManager()->set_current_text("Enter Base Tile:");
	base_tile_id = std::stoi(Environment::get().getInputManager()->start_text_input());
}

UIHandler_Editor::UIHandler_Editor(){

}

void UIHandler_Editor::button_new_map() {
	Environment::get().getUIManager()->set_current_text("Are you sure you want to create a new map?");
	Environment::get().getUIManager()->push_confirmation(create_new_map, nullptr);
}