#include <SDL.h>

#include <vector>

#include "UIHandler.h"

#include "UIHandler.h"

#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#define ELEMENT_WIDTH 32
#define ELEMENT_HEIGHT 32
#define ELEMENT_ROW_SIZE 8
#define ELEMENT_AREA_WIDTH ELEMENT_ROW_SIZE * ELEMENT_WIDTH

namespace UI {

	struct Element_Area {
		SDL_Rect background;
		SDL_Color color;
		SDL_Rect element_area;
		SDL_Rect selection_area;
	};

	enum {
		STATE_IDLE,
		STATE_CONFIRM,
		STATE_DENY,
		STATE_WAITING,
	};

}

class UIManager {
public:
	typedef void(*Callback)(void);

	UIManager();
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
	bool check(const int &mouse_x, const int &mouse_y);
	void render();

	void set_state(int flag);
	void set_current_text(std::string text);

	void push_confirmation(Callback on_confirm = nullptr, Callback on_deny = nullptr);
	void pop_confirmation();

	// returns element id ( -1 if mouse is not hovering an element)
	int select_element(const int &mouse_x, const int &mouse_y);
private:
	int _state;

	bool _element_selected;
	std::string _element_type;
	int _element_index;

	Text _current_text;

	Callback _on_confirm;
	Callback _on_deny;

	Element_Area _element_area;

	UIHandler *_uiHandler;
};

#endif