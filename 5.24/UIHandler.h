#include <SDL.h>

#include <map>
#include <string>

#include "Button.h"
#include "Button_Pressable.h"

#ifndef UI_HANDLER_H
#define UI_HANDLER_H

using namespace UI;

class UIHandler {
public:
	UIHandler() {}
	~UIHandler();

	void confirm_yes();
	void confirm_no();

	friend class UIManager;
private:
	std::map<std::string, Button *> _buttons;
};

#endif