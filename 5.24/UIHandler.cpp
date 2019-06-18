#include "UIHandler.h"

UIHandler::~UIHandler() {
	for (auto it = _buttons.begin(); it != _buttons.end(); it++) {
		delete *it;
	}
}