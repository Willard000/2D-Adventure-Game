#include <SDL.h>

#include <vector>

#include "UIHandler.h"

#include "UIHandler.h"

#ifndef UI_MANAGER_H
#define UI_MANAGER_H

class UIManager {
public:
	UIManager(bool editor);
	~UIManager();

	void update();
	void check(const int &mouse_x, const int &mouse_y);
	void render();
private:
	UIHandler *_uiHandler;
};

#endif