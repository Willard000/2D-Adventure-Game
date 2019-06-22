#include <SDL.h>

#include <vector>

#include "UIHandler.h"

#include "UIHandler.h"

#ifndef UI_MANAGER_H
#define UI_MANAGER_H

namespace UI {
	enum {
		STATE_IDLE = 1,
		STATE_CONFIRM,
		STATE_DENY,
		STATE_WAITING
	};
}

class UIManager {
public:
	typedef void(*Callback)(void);

	UIManager(bool editor);
	~UIManager();

	template <class T>
	void add_button(
		T *obj,
		void (T::*MemberFunction)(void),
		std::string text = { "TEXT" },
		SDL_Rect rect = { 0, 0, 80, 20 },
		int font_size = 16,
		Uint32 wrap_size = 1000,
		SDL_Color text_color = { 255, 255, 255, 255 },
		SDL_Color color = { 30, 30, 30, 120 })
	{
		_uiHandler->_buttons[text] = (new Button_Pressable<T>(obj, MemberFunction, text, text_color, font_size, wrap_size, rect, color));
	}

	void remove_button(std::string key);

	int update();
	void check(const int &mouse_x, const int &mouse_y);
	void render();

	void set_state(int flag);
	void set_current_text(std::string text);

	void push_confirmation(Callback on_confirm = nullptr, Callback on_deny = nullptr);
	void pop_confirmation();
private:
	int _state;

	Text _current_text;

	Callback _on_confirm;
	Callback _on_deny;

	UIHandler *_uiHandler;
};

#endif