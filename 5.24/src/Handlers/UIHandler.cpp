#include "UIHandler.h"

#include "Environment.h"
#include "UIManager.h"
#include "ResourceManager.h"

#include "Entity.h"
#include "PlayerComponent.h"


using namespace UI;

void UI::confirm_yes() {
	Environment::get().get_ui_manager()->set_state(STATE_CONFIRM);
}

void UI::confirm_no() {
	Environment::get().get_ui_manager()->set_state(STATE_DENY);
}

void UI::equip_item() {
	GetPlayer(Environment::get().get_resource_manager()->get_player())->inventory.equip_item();
}

void UI::use_item() {
	GetPlayer(Environment::get().get_resource_manager()->get_player())->inventory.use_item();
}

void UI::drop_item() {
	GetPlayer(Environment::get().get_resource_manager()->get_player())->inventory.drop_item();
}