#include "UIHandler.h"

#include "Environment.h"
#include "UIManager.h"
#include "WindowManager.h"
#include "LogManager.h"

UIHandler::~UIHandler() {
	for (auto it = _buttons.begin(); it != _buttons.end(); it++) {
		delete it->second;
	}
}

void UIHandler::confirm_yes() {
	Environment::get().getUIManager()->set_state(STATE_CONFIRM);
}

void UIHandler::confirm_no() {
	Environment::get().getUIManager()->set_state(STATE_DENY);
}