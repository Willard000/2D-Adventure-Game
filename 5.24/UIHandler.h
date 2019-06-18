#include <SDL.h>

#include <vector>

#include "Button.h"

#ifndef UI_HANDLER_H
#define UI_HANDLER_H

using namespace UI;

class UIHandler {
public:
	UIHandler() {}
	~UIHandler();

	template <class T>
	void addButton(T *obj, void (T::*MemberFunction)(void), SDL_Rect rect = { 0, 0, 120, 30 }, SDL_Color color = { 255, 0, 0, 100 }) {
		_buttons.push_back(new ButtonPress<T>(obj, MemberFunction, rect, color));
	}

	friend class UIManager;
private:
	std::vector<Button *> _buttons;
};

#endif