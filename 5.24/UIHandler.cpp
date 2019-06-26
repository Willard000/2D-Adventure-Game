#include "UIHandler.h"

#include "Environment.h"
#include "UIManager.h"


using namespace UI;

void UI::confirm_yes() {
	Environment::get().getUIManager()->set_state(STATE_CONFIRM);
}

void UI::confirm_no() {
	Environment::get().getUIManager()->set_state(STATE_DENY);
}