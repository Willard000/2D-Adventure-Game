#include "UIHandler.h"

#ifndef UI_HANDLER_EDITOR_H
#define UI_HANDLER_EDITOR_H

class UIHandler_Editor : public UIHandler {
public:
	UIHandler_Editor();

	void button_new_map();
	void button_save_map();
	void button_load_map();
private:
};
#endif