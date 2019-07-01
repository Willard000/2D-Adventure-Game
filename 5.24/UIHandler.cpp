#include "UIHandler.h"

#include "Environment.h"
#include "UIManager.h"


using namespace UI;

void UI::confirm_yes() {
	Environment::get().get_ui_manager()->set_state(STATE_CONFIRM);
}

void UI::confirm_no() {
	Environment::get().get_ui_manager()->set_state(STATE_DENY);
}